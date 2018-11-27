
#include <linux/module.h>
#include "ktf.h"

MODULE_LICENSE("GPL");

KTF_INIT();

TEST(simple, t1)
{
	EXPECT_TRUE(true);
}

static void add_tests(void)
{
	ADD_TEST(t1);
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
