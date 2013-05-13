/*
 * copy_table.h
 *
 *  Created on: 2011-9-28
 *      Author: djyos
 */

#ifndef COPY_TABLE_H_
#define COPY_TABLE_H_

#ifdef __cplusplus
extern "C" {
#endif

struct __copy_record{
	u32 load_start_address;
	u32 run_start_address;
	u32 size;
	u32 type;	//0:zero init;    1:copy
};

struct copy_table{
	u32 record_size;
	u32 record_cnt;
	struct __copy_record record[1];
};

#ifdef __cplusplus
}
#endif

#endif /* COPY_TABLE_H_ */
