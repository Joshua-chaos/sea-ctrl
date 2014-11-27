/*	
						ZheJiang University
			Control Science & Engineering(CSE)
							file information
	file name:error.h
	version:v1.0
	lastdate:2013/4/12  y/m/d
	author£ºjiaxuechao
	description:define some error code for debugger
*/

//can communication error
#ifndef _ERROR_H_
#define _ERROR_H_

//for c++ compatible
#ifdef  __cplusplus
extern "C" {
#endif

typedef unsigned char     error_code;
#define none_err 						0
#define frame_len_err 			1
#define buffer_full_err  		2
#define buf_timeout_err     3
#define sent_busy_err      	4
#define sent_timeout_err    5
#define sent_finished       6 //none_err	
#define read_wait_err       7 //none_err
#define bus_busy_err      	8
#define rdbk_timeout_err	  9
#define bad_cmd_err         10
#define bad_recformat_err   11
#define bad_recid_err       12
#define bad_rechead_err			13	
#define bad_recindex_err    14


#ifdef __cplusplus
    }
#endif
#endif
