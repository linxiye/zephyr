

#define DT_DRV_COMPAT st_stm32_fsmc_sram

#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <soc.h>
#include <errno.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(st_stm32_fsmc_sram, CONFIG_MEMC_LOG_LEVEL);
#include <zephyr/sys/printk.h>

/** FMC NOR/PSRAM controller bank configuration fields. */
struct memc_stm32_fsmc_sram_bank_config {
	FMC_NORSRAM_InitTypeDef init;
	FSMC_NORSRAM_TimingTypeDef timing;
	FSMC_NORSRAM_TimingTypeDef timing_ext;
};

/** FMC NOR/PSRAM controller configuration fields. */
struct memc_stm32_fsmc_sram_config {
	const struct memc_stm32_fsmc_sram_bank_config *banks;
	size_t banks_len;
};


static int memc_stm32_fsmc_sram_init(const struct memc_stm32_fsmc_sram_bank_config *config)
{
	SRAM_HandleTypeDef hsram;
    FSMC_NORSRAM_TimingTypeDef Timing = {0};
    FSMC_NORSRAM_TimingTypeDef ExtTiming = {0};

    hsram.Instance = FSMC_NORSRAM_DEVICE;
    hsram.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
	
    memcpy(&hsram.Init, &config->init, sizeof(hsram.Init));

    if(config->init.NSBank == 1)
        hsram.Init.NSBank = FSMC_NORSRAM_BANK1;
    else if(config->init.NSBank == 2)
        hsram.Init.NSBank = FSMC_NORSRAM_BANK2;
    else if(config->init.NSBank == 3)
        hsram.Init.NSBank = FSMC_NORSRAM_BANK3;
    else if(config->init.NSBank == 4)
        hsram.Init.NSBank = FSMC_NORSRAM_BANK4;
    else{
        printk("fsmc sram init bank error!!");
		return 1;
    }

    memcpy(&Timing, &config->timing, sizeof(Timing));
    memcpy(&ExtTiming, &config->timing_ext, sizeof(ExtTiming));
    HAL_SRAM_Init(&hsram, &Timing, &ExtTiming);
    return 0;
}

static int memc_stm32_fsmc_init(const struct device *dev)
{
    const struct memc_stm32_fsmc_sram_config *config = dev->config;

    for(int i = 0; i < config->banks_len; i++)
    {
        memc_stm32_fsmc_sram_init(&config->banks[i]);
    }

    return 0;
}



#define BANK_CONFIG(node_id)                                                \
	{ .init = {                                                             \
	    .NSBank = DT_REG_ADDR(node_id),                                     \
	    .DataAddressMux = DT_PROP_BY_IDX(node_id, st_control, 0),           \
	    .MemoryType = DT_PROP_BY_IDX(node_id, st_control, 1),               \
	    .MemoryDataWidth = DT_PROP_BY_IDX(node_id, st_control, 2),          \
	    .BurstAccessMode = DT_PROP_BY_IDX(node_id, st_control, 3),          \
	    .WaitSignalPolarity = DT_PROP_BY_IDX(node_id, st_control, 4),       \
	    .WrapMode = DT_PROP_BY_IDX(node_id, st_control, 5),                 \
	    .WaitSignalActive = DT_PROP_BY_IDX(node_id, st_control, 6),         \
	    .WriteOperation = DT_PROP_BY_IDX(node_id, st_control, 7),           \
	    .WaitSignal = DT_PROP_BY_IDX(node_id, st_control, 8),               \
	    .ExtendedMode = DT_PROP_BY_IDX(node_id, st_control, 9),             \
	    .AsynchronousWait = DT_PROP_BY_IDX(node_id, st_control, 10),        \
	    .WriteBurst = DT_PROP_BY_IDX(node_id, st_control, 11),              \
	    .PageSize = DT_PROP_BY_IDX(node_id, st_control, 12),                \
	  },                                                                    \
	  .timing = {                                                           \
	    .AddressSetupTime = DT_PROP_BY_IDX(node_id, st_timing, 0),          \
	    .AddressHoldTime =  DT_PROP_BY_IDX(node_id, st_timing, 1),          \
	    .DataSetupTime = DT_PROP_BY_IDX(node_id, st_timing, 2),             \
	    .BusTurnAroundDuration = DT_PROP_BY_IDX(node_id, st_timing, 3),     \
	    .CLKDivision = DT_PROP_BY_IDX(node_id, st_timing, 4),               \
	    .DataLatency = DT_PROP_BY_IDX(node_id, st_timing, 5),               \
	    .AccessMode = DT_PROP_BY_IDX(node_id, st_timing, 6),                \
	  },                                                                    \
	  .timing_ext = {                                                       \
	    .AddressSetupTime = DT_PROP_BY_IDX(node_id, st_timing_ext, 0),      \
	    .AddressHoldTime =  DT_PROP_BY_IDX(node_id, st_timing_ext, 1),      \
	    .DataSetupTime = DT_PROP_BY_IDX(node_id, st_timing_ext, 2),         \
	    .BusTurnAroundDuration = DT_PROP_BY_IDX(node_id, st_timing_ext, 3), \
	    .AccessMode = DT_PROP_BY_IDX(node_id, st_timing_ext, 4),            \
	  }                                                                     \
	},

/** SRAM bank/s configuration. */
static const struct memc_stm32_fsmc_sram_bank_config bank_config[] = {
	DT_INST_FOREACH_CHILD(0, BANK_CONFIG)
};

/** SRAM configuration. */
static const struct memc_stm32_fsmc_sram_config config = {
	.banks = bank_config,
	.banks_len = ARRAY_SIZE(bank_config),
};

DEVICE_DT_INST_DEFINE(0, memc_stm32_fsmc_init, NULL,
	      NULL, &config, POST_KERNEL, CONFIG_MEMC_INIT_PRIORITY, NULL);
