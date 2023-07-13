/*
 * Copyright (c) 2020 Teslabs Engineering S.L.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_DT_BINDINGS_MEMORY_CONTROLLER_STM32_FSMC_SRAM_H_
#define ZEPHYR_INCLUDE_DT_BINDINGS_MEMORY_CONTROLLER_STM32_FSMC_SRAM_H_

/** @defgroup FSMC_Data_Address_Bus_Multiplexing FSMC Data Address Bus Multiplexing
  * @{
  */
#define FSMC_DATA_ADDRESS_MUX_DISABLE            (0x00000000U)
#define FSMC_DATA_ADDRESS_MUX_ENABLE             (0x00000002U)

/** @defgroup FSMC_Memory_Type FSMC Memory Type
  * @{
  */
#define FSMC_MEMORY_TYPE_SRAM                    (0x00000000U)
#define FSMC_MEMORY_TYPE_PSRAM                   (0x00000004U)
#define FSMC_MEMORY_TYPE_NOR                     (0x00000008U)

/** @defgroup FSMC_NORSRAM_Data_Width FSMC NORSRAM Data Width
  * @{
  */
#define FSMC_NORSRAM_MEM_BUS_WIDTH_8             (0x00000000U)
#define FSMC_NORSRAM_MEM_BUS_WIDTH_16            (0x00000010U)
#define FSMC_NORSRAM_MEM_BUS_WIDTH_32            (0x00000020U)

/** @defgroup FSMC_NORSRAM_Flash_Access FSMC NOR/SRAM Flash Access
  * @{
  */
#define FSMC_NORSRAM_FLASH_ACCESS_ENABLE         (0x00000040U)
#define FSMC_NORSRAM_FLASH_ACCESS_DISABLE        (0x00000000U)

/** @defgroup FSMC_Burst_Access_Mode FSMC Burst Access Mode
  * @{
  */
#define FSMC_BURST_ACCESS_MODE_DISABLE           (0x00000000U)
#define FSMC_BURST_ACCESS_MODE_ENABLE            (0x00000100U)

/** @defgroup FSMC_Wait_Signal_Polarity FSMC Wait Signal Polarity
  * @{
  */
#define FSMC_WAIT_SIGNAL_POLARITY_LOW            (0x00000000U)
#define FSMC_WAIT_SIGNAL_POLARITY_HIGH           (0x00000200U)

/** @defgroup FSMC_Wrap_Mode FSMC Wrap Mode
  * @note  These values are available only for the STM32F405/415/407/417xx devices.
  * @{
  */
#define FSMC_WRAP_MODE_DISABLE                   (0x00000000U)
#define FSMC_WRAP_MODE_ENABLE                    (0x00000400U)

/** @defgroup FSMC_Wait_Timing FSMC Wait Timing
  * @{
  */
#define FSMC_WAIT_TIMING_BEFORE_WS               (0x00000000U)
#define FSMC_WAIT_TIMING_DURING_WS               (0x00000800U)

/** @defgroup FSMC_Write_Operation FSMC Write Operation
  * @{
  */
#define FSMC_WRITE_OPERATION_DISABLE             (0x00000000U)
#define FSMC_WRITE_OPERATION_ENABLE              (0x00001000U)

/** @defgroup FSMC_Wait_Signal FSMC Wait Signal
  * @{
  */
#define FSMC_WAIT_SIGNAL_DISABLE                 (0x00000000U)
#define FSMC_WAIT_SIGNAL_ENABLE                  (0x00002000U)

/** @defgroup FSMC_Extended_Mode FSMC Extended Mode
  * @{
  */
#define FSMC_EXTENDED_MODE_DISABLE               (0x00000000U)
#define FSMC_EXTENDED_MODE_ENABLE                (0x00004000U)

/** @defgroup FSMC_AsynchronousWait FSMC Asynchronous Wait
  * @{
  */
#define FSMC_ASYNCHRONOUS_WAIT_DISABLE           (0x00000000U)
#define FSMC_ASYNCHRONOUS_WAIT_ENABLE            (0x00008000U)

/** @defgroup FSMC_Page_Size FSMC Page Size
  * @{
  */
#define FSMC_PAGE_SIZE_NONE                      (0x00000000U)
#define FSMC_PAGE_SIZE_128                       FSMC_BCR1_CPSIZE_0
#define FSMC_PAGE_SIZE_256                       FSMC_BCR1_CPSIZE_1
#define FSMC_PAGE_SIZE_512                       (FSMC_BCR1_CPSIZE_0\
                                                 | FSMC_BCR1_CPSIZE_1)
#define FSMC_PAGE_SIZE_1024                      FSMC_BCR1_CPSIZE_2

/** @defgroup FSMC_Write_Burst FSMC Write Burst
  * @{
  */
#define FSMC_WRITE_BURST_DISABLE                 (0x00000000U)
#define FSMC_WRITE_BURST_ENABLE                  (0x00080000U)

/** @defgroup FSMC_Continous_Clock FSMC Continuous Clock
  * @note  These values are available only for the STM32F412Vx/Zx/Rx devices.
  * @{
  */
#define FSMC_CONTINUOUS_CLOCK_SYNC_ONLY          (0x00000000U)
#define FSMC_CONTINUOUS_CLOCK_SYNC_ASYNC         (0x00100000U)

/** @defgroup FSMC_Access_Mode FSMC Access Mode
  * @{
  */
#define FSMC_ACCESS_MODE_A                       (0x00000000U)
#define FSMC_ACCESS_MODE_B                       (0x10000000U)
#define FSMC_ACCESS_MODE_C                       (0x20000000U)
#define FSMC_ACCESS_MODE_D                       (0x30000000U)

/** @defgroup FSMC_NORSRAM_Bank FSMC NOR/SRAM Bank
  * @{
  */
#define FSMC_NORSRAM_BANK1                       (0x00000000U)
#define FSMC_NORSRAM_BANK2                       (0x00000002U)
#define FSMC_NORSRAM_BANK3                       (0x00000004U)
#define FSMC_NORSRAM_BANK4                       (0x00000006U)

#endif
