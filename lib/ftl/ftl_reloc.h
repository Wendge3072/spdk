/*   SPDX-License-Identifier: BSD-3-Clause
 *   Copyright (c) Intel Corporation.
 *   All rights reserved.
 */

#ifndef FTL_RELOC_H
#define FTL_RELOC_H

#include "spdk/stdinc.h"
#include "spdk/uuid.h"
#include "spdk/thread.h"
#include "spdk/util.h"
#include "spdk/likely.h"
#include "spdk/queue.h"
#include "spdk/ftl.h"
#include "spdk/bdev.h"

#include "ftl_internal.h"
#include "ftl_io.h"
#include "ftl_trace.h"
#include "ftl_nv_cache.h"
#include "ftl_writer.h"
#include "ftl_layout.h"
#include "ftl_sb.h"
#include "ftl_l2p.h"
#include "utils/ftl_bitmap.h"
#include "utils/ftl_log.h"


#define FTL_RELOC_THROTTLE_INTERVAL_MS 50

#define FTL_COMP_IDLE_THRESHOLD_SEC (512 * MiB) /* 512MB */

struct ftl_reloc;
struct ftl_band_reloc;

/* TODO: Should probably change the move naming nomenclature to something more descriptive */
enum ftl_reloc_move_state {
	FTL_RELOC_STATE_READ = 0,
	FTL_RELOC_STATE_PIN,
	FTL_RELOC_STATE_WRITE,
	FTL_RELOC_STATE_WAIT,
	FTL_RELOC_STATE_HALT,

	FTL_RELOC_STATE_MAX
};

struct ftl_reloc_move {
	/* FTL device */
	struct spdk_ftl_dev *dev;

	struct ftl_reloc *reloc;

	/* Request for doing IO */
	struct ftl_rq *rq;

	/* Move state (read, write) */
	enum ftl_reloc_move_state state;

	/* Entry of circular list */
	TAILQ_ENTRY(ftl_reloc_move) qentry;
};

struct ftl_reloc {
	/* Device associated with relocate */
	struct spdk_ftl_dev *dev;

	/* Indicates relocate is about to halt */
	bool halt;

	/* Band which are read to relocate */
	struct ftl_band *band;

	/* Bands already read, but waiting for finishing GC */
	TAILQ_HEAD(, ftl_band) band_done;
	size_t band_done_count;

	/* Flags indicating reloc is waiting for a new band */
	bool band_waiting;

	/* Maximum number of IOs per band */
	size_t max_qdepth;

	/* Queue of free move objects */
	struct ftl_reloc_move *move_buffer;

	uint64_t search_band_time;

	/* Array of movers queue for each state */
	TAILQ_HEAD(, ftl_reloc_move) move_queue[FTL_RELOC_STATE_MAX];

	/* Throttle for the reloc */
	struct {
		uint64_t rInterval_tsc;
		uint64_t rStart_tsc;
		uint64_t rBlocks_submitted;
		uint64_t rBlocks_submitted_limit;
	} rThrottle;

	double Max_invalidity;

};


#endif /* FTL_RELOC_H */