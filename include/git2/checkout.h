/*
 * Copyright (C) 2012 the libgit2 contributors
 *
 * This file is part of libgit2, distributed under the GNU GPL v2 with
 * a Linking Exception. For full terms see the included COPYING file.
 */
#ifndef INCLUDE_git_checkout_h__
#define INCLUDE_git_checkout_h__

#include "common.h"
#include "types.h"
#include "indexer.h"


/**
 * @file git2/checkout.h
 * @brief Git checkout routines
 * @defgroup git_checkout Git checkout routines
 * @ingroup Git
 * @{
 */
GIT_BEGIN_DECL

enum {
	GIT_CHECKOUT_DEFAULT			= (1 << 0),
	GIT_CHECKOUT_OVERWRITE_MODIFIED	= (1 << 1),
	GIT_CHECKOUT_CREATE_MISSING		= (1 << 2),
	GIT_CHECKOUT_REMOVE_UNTRACKED	= (1 << 3),
};

/* Use zeros to indicate default settings */
typedef struct git_checkout_opts {
	unsigned int checkout_strategy; /* default: GIT_CHECKOUT_DEFAULT */
	int disable_filters;
	int dir_mode; /* default is 0755 */
	int file_mode; /* default is 0644 */
	int file_open_flags; /* default is O_CREAT | O_TRUNC | O_WRONLY */

	/* Optional callback to notify the consumer of files that
	 * haven't be checked out because a modified version of them
	 * exist in the working directory.
	 *
	 * When provided, this callback will be invoked when the flag
	 * GIT_CHECKOUT_OVERWRITE_MODIFIED isn't part of the checkout strategy.
	 */
	int (* skipped_notify_cb)(
		const char *skipped_file,
		const git_oid *blob_oid,
		int file_mode,
		void *payload);

	void *notify_payload;

	/* when not NULL, arrays of fnmatch pattern specifying 
	 * which paths should be taken into account
	 */
	git_strarray paths; 
} git_checkout_opts;

/**
 * Updates files in the index and the working tree to match the content of the
 * commit pointed at by HEAD.
 *
 * @param repo repository to check out (must be non-bare)
 * @param opts specifies checkout options (may be NULL)
 * @param stats structure through which progress information is reported
 * @return 0 on success, GIT_ERROR otherwise (use giterr_last for information
 * about the error)
 */
GIT_EXTERN(int) git_checkout_head(
	git_repository *repo,
	git_checkout_opts *opts,
	git_indexer_stats *stats);

/**
 * Updates files in the working tree to match the content of the index.
 *
 * @param repo repository to check out (must be non-bare)
 * @param opts specifies checkout options (may be NULL)
 * @param stats structure through which progress information is reported
 * @return 0 on success, GIT_ERROR otherwise (use giterr_last for information
 * about the error)
 */
GIT_EXTERN(int) git_checkout_index(
	git_repository *repo,
	git_checkout_opts *opts,
	git_indexer_stats *stats);

/**
 * Updates files in the index and working tree to match the content of the
 * tree pointed at by the treeish.
 *
 * @param repo repository to check out (must be non-bare)
 * @param treeish a commit, tag or tree which content will be used to update
 * the working directory
 * @param opts specifies checkout options (may be NULL)
 * @param stats structure through which progress information is reported
 * @return 0 on success, GIT_ERROR otherwise (use giterr_last for information
 * about the error)
 */
GIT_EXTERN(int) git_checkout_tree(
	git_repository *repo,
	git_object *treeish,
	git_checkout_opts *opts,
	git_indexer_stats *stats);

/** @} */
GIT_END_DECL
#endif
