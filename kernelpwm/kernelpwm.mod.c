#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x3f584fcd, "module_layout" },
	{ 0xfd958c00, "param_ops_int" },
	{ 0xd0053a2e, "kthread_stop" },
	{ 0xf08d7414, "wake_up_process" },
	{ 0x748163a6, "kthread_create_on_node" },
	{ 0xfe990052, "gpio_free" },
	{ 0x127f5033, "gpiod_direction_output_raw" },
	{ 0x47229b5c, "gpio_request" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0xb3f7646e, "kthread_should_stop" },
	{ 0x12a38747, "usleep_range" },
	{ 0xe27518da, "gpiod_set_raw_value" },
	{ 0x2a941df5, "gpio_to_desc" },
	{ 0x7c32d0f0, "printk" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
	{ 0x2196324, "__aeabi_idiv" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "A0B74623420E56BC369842D");
