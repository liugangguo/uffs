/*
  This file is part of UFFS, the Ultra-low-cost Flash File System.
  
  Copyright (C) 2005-2009 Ricky Zheng <ricky_gz_zheng@yahoo.co.nz>

  UFFS is free software; you can redistribute it and/or modify it under
  the GNU Library General Public License as published by the Free Software 
  Foundation; either version 2 of the License, or (at your option) any
  later version.

  UFFS is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
  or GNU Library General Public License, as applicable, for more details.
 
  You should have received a copy of the GNU General Public License
  and GNU Library General Public License along with UFFS; if not, write
  to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
  Boston, MA  02110-1301, USA.

  As a special exception, if other files instantiate templates or use
  macros or inline functions from this file, or you compile this file
  and link it with other works to produce a work based on this file,
  this file does not by itself cause the resulting work to be covered
  by the GNU General Public License. However the source code for this
  file must still be made available in accordance with section (3) of
  the GNU General Public License v2.
 
  This exception does not invalidate any other reasons why a work based
  on this file might be covered by the GNU General Public License.
*/

/** 
 * \file uffs_buf.h
 * \brief page buffers
 * \author Ricky Zheng
 */

#ifndef UFFS_BUF_H
#define UFFS_BUF_H

#include "uffs/uffs_types.h"
#include "uffs/uffs_device.h"
#include "uffs/uffs_tree.h"
#include "uffs/uffs_core.h"

#ifdef __cplusplus
extern "C"{
#endif
	
#define CLONE_BUF_MARK		0xffff

#define UFFS_BUF_EMPTY		0
#define UFFS_BUF_VALID		1
#define UFFS_BUF_DIRTY		2

/** uffs page buffer */
struct uffs_BufSt{
	struct uffs_BufSt *next;	//!< link to next buffer
	struct uffs_BufSt *prev;	//!< link to previous buffer
	struct uffs_BufSt *next_dirty;
	struct uffs_BufSt *prev_dirty;
	u8 type;					//!< file, dir, or data
	u16 father;					//!< father serial
	u16 serial;					//!< serial 
	u16 page_id;					//!< page id 
	u16 mark;					//!< #UFFS_BUF_EMPTY, #UFFS_BUF_VALID, or #UFFS_BUF_DIRTY ?
	u16 ref_count;				//!< reference counter
	u16 data_len;				//!< length of data
	u8 * data;					//!< data buffer
	u8 * ecc;					//!< ecc buffer
};


URET uffs_BufInit(struct uffs_DeviceSt *dev, int buf_max, int dirty_buf_max);
URET uffs_BufReleaseAll(struct uffs_DeviceSt *dev);

uffs_Buf * uffs_BufGet(struct uffs_DeviceSt *dev, u16 father, u16 serial, u16 page_id);
uffs_Buf *uffs_BufNew(struct uffs_DeviceSt *dev, u8 type, u16 father, u16 serial, u16 page_id);
uffs_Buf *uffs_BufGetEx(struct uffs_DeviceSt *dev, u8 type, TreeNode *node, u16 page_id);
uffs_Buf * uffs_BufFind(uffs_Device *dev, u16 father, u16 serial, u16 page_id);

URET uffs_BufPut(uffs_Device *dev, uffs_Buf *buf);

void uffs_BufIncRef(uffs_Buf *buf);
void uffs_BufDecRef(uffs_Buf *buf);
URET uffs_BufWrite(struct uffs_DeviceSt *dev, uffs_Buf *buf, void *data, u32 ofs, u32 len);
URET uffs_BufRead(struct uffs_DeviceSt *dev, uffs_Buf *buf, void *data, u32 ofs, u32 len);
void uffs_BufSetMark(uffs_Buf *buf, int mark);

URET uffs_BufFlush(struct uffs_DeviceSt *dev);
URET uffs_BufFlushEx(struct uffs_DeviceSt *dev, UBOOL force_block_recover);

URET uffs_BufFlushGroup(struct uffs_DeviceSt *dev, u16 father, u16 serial);
URET uffs_BufFlushGroupEx(struct uffs_DeviceSt *dev, u16 father, u16 serial, UBOOL force_block_recover);
int uffs_BufFindFreeGroupSlot(struct uffs_DeviceSt *dev);
int uffs_BufFindGroupSlot(struct uffs_DeviceSt *dev, u16 father, u16 serial);
URET uffs_BufFlushMostDirtyGroup(struct uffs_DeviceSt *dev);
URET uffs_BufFlushGroupMatchFather(struct uffs_DeviceSt *dev, u16 father);

URET uffs_BufFlushAll(struct uffs_DeviceSt *dev);

UBOOL uffs_BufIsAllFree(struct uffs_DeviceSt *dev);
UBOOL uffs_BufIsAllEmpty(struct uffs_DeviceSt *dev);
URET uffs_BufSetAllEmpty(struct uffs_DeviceSt *dev);

uffs_Buf * uffs_BufClone(struct uffs_DeviceSt *dev, uffs_Buf *buf);
void uffs_BufFreeClone(uffs_Device *dev, uffs_Buf *buf);

URET uffs_LoadPhiDataToBuf(uffs_Device *dev, uffs_Buf *buf, u32 block, u32 page);
URET uffs_LoadPhiDataToBufEccUnCare(uffs_Device *dev, uffs_Buf *buf, u32 block, u32 page);

void uffs_BufInspect(uffs_Device *dev);

#ifdef __cplusplus
}
#endif


#endif
