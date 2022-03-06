/**
 * @file process.h
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 主控头文件
 * @version 0.1
 * @date 2022-02-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __PROCESS_H_
#define __PROCESS_H_

#ifdef  __cplusplus
extern "C" {
#endif

void Process_Main_Init(void);

void Process_Main_Run(void);

void Process_Circular_Run(void);

#ifdef  __cplusplus
}

#endif

#endif // !__PROCESS_H_
