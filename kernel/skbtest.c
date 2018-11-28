/*
 * Copyright (c) 2009, 2018, Oracle and/or its affiliates. All rights reserved.
 *    Author: Alan Maguire <alan.maguire@oracle.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * skbtest.c: Series of tests of the skb data structure/functions which
 * also demonstrate KTF functionality.
 */

#include <linux/module.h>
#include <linux/skbuff.h>
#include "ktf.h"

MODULE_LICENSE("GPL");

KTF_INIT();

/**
 * alloc_skb_sizes()
 *
 * ensure initial skb state is as expected for allocations of various sizes.
 *  - head == data
 *  - end >= tail + size
 *  - len == data_len == 0
 *  - nr_frags == 0
 *
 **/
TEST(skb, alloc_skb_sizes)
{
	unsigned int i, sizes[] = { 127, 260, 320, 550, 1028, 2059 };
	struct sk_buff *skb = NULL;

	for (i = 0; i < ARRAY_SIZE(sizes); i++) {
		skb = alloc_skb(sizes[i], GFP_KERNEL);

		ASSERT_ADDR_NE_GOTO(skb, 0, done);
		ASSERT_ADDR_EQ_GOTO(skb->head, skb->data, done);
		/*
		 * skb->end will be aligned and include overhead of shared
		 * info.
		 */
		ASSERT_TRUE_GOTO(skb->end >= skb->tail + sizes[i], done);
		ASSERT_TRUE_GOTO(skb->tail == skb->data - skb->head, done);
		ASSERT_TRUE_GOTO(skb->len == 0, done);
		ASSERT_TRUE_GOTO(skb->data_len == 0, done);
		ASSERT_TRUE_GOTO(skb_shinfo(skb)->nr_frags == 0, done);
		kfree_skb(skb);
		skb = NULL;
	}

done:
	kfree_skb(skb);
}

struct task_struct *alloc_skb_nomem_task;

KTF_RETURN_PROBE(kmem_cache_alloc_node, kmem_cache_alloc_nodehandler)
{
	struct sk_buff *retval = (void *)KTF_RETURN_VALUE();
	struct kmem_cache **cache;

	/* We only want alloc failures for this task! */
	if (alloc_skb_nomem_task != current)
		return 0;

	/* skbuf_head_cache is private to skbuff.c */
	cache = ktf_find_symbol(NULL, "skbuff_head_cache");
	if (!cache || !*cache || !retval)
		return 0;

	kmem_cache_free(*cache, retval);
	KTF_SET_RETURN_VALUE(0);

	return 0;
}

TEST(skb, alloc_skb_nomem)
{
	struct sk_buff *skb = NULL;

	alloc_skb_nomem_task = current;

	ASSERT_INT_EQ_GOTO(KTF_REGISTER_RETURN_PROBE(kmem_cache_alloc_node,
			   kmem_cache_alloc_nodehandler), 0, done);

	skb = alloc_skb(128, GFP_KERNEL);
	ASSERT_ADDR_EQ_GOTO(skb, 0, done);

	alloc_skb_nomem_task = NULL;
done:
	KTF_UNREGISTER_RETURN_PROBE(kmem_cache_alloc_node,
				    kmem_cache_alloc_nodehandler);
	kfree_skb(skb);
}

static void add_tests(void)
{
	ADD_TEST(alloc_skb_sizes);
	ADD_TEST(alloc_skb_nomem);
}

static int __init skbtest_init(void)
{
	add_tests();
	return 0;
}
static void __exit skbtest_exit(void)
{
	KTF_CLEANUP();
}

module_init(skbtest_init);
module_exit(skbtest_exit);
