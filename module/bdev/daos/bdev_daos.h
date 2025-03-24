/*   SPDX-License-Identifier: BSD-3-Clause
 *   Copyright (c) Intel Corporation.
 *   All rights reserved.
 *   Copyright (c) croit GmbH.
 *   All rights reserved.
 */

#ifndef SPDK_BDEV_DAOS_H
#define SPDK_BDEV_DAOS_H

#include "spdk/stdinc.h"
#include "spdk/bdev.h"

typedef void (*spdk_delete_daos_complete)(void *cb_arg, int bdeverrno);

int create_bdev_daos(struct spdk_bdev **bdev, const char *name, const struct spdk_uuid *uuid,
		     const char *pool, const char *cont,
		     uint64_t num_blocks, uint32_t block_size);

void delete_bdev_daos(struct spdk_bdev *bdev, spdk_delete_daos_complete cb_fn, void *cb_arg);

#endif /* SPDK_BDEV_DAOS_H */
