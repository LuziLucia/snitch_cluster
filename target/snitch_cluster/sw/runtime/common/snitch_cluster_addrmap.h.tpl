// Copyright 2023 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#define CLUSTER_TCDM_BASE_ADDR ${hex(cfg['cluster']['cluster_base_addr'])}
#define CLUSTER_BOOTROM_BASE_ADDR (CLUSTER_TCDM_BASE_ADDR + ${hex(cfg['cluster']['tcdm']['size'] * 1024)})
#define CLUSTER_PERIPH_BASE_ADDR (CLUSTER_BOOTROM_BASE_ADDR + 4 * 1024)
#define CLUSTER_ZERO_MEM_START_ADDR (CLUSTER_PERIPH_BASE_ADDR + ${hex(cfg['cluster']['cluster_periph_size'] * 1024)})
#define CLUSTER_ZERO_MEM_END_ADDR (CLUSTER_ZERO_MEM_START_ADDR + ${hex(cfg['cluster']['zero_mem_size'] * 1024)})
#define CLINT_BASE_ADDR ${hex(cfg['peripherals']['clint']['address'])}
#define ALIAS_TCDM_BASE_ADDR ${hex(cfg['cluster']['alias_region_base'])}
#define ALIAS_BOOTROM_BASE_ADDR (ALIAS_TCDM_BASE_ADDR + ${hex(cfg['cluster']['tcdm']['size'] * 1024)})
#define ALIAS_PERIPH_BASE_ADDR (ALIAS_BOOTROM_BASE_ADDR + 4 * 1024)
#define ALIAS_ZERO_MEM_START_ADDR (ALIAS_PERIPH_BASE_ADDR + ${hex(cfg['cluster']['cluster_periph_size'] * 1024)})
