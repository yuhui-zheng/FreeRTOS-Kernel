/*
 * FreeRTOS Kernel V10.3.1
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/*
 * Implementation of the wrapper functions used to raise the processor privilege
 * before calling a standard FreeRTOS API function.
 */

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#define MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "event_groups.h"
#include "stream_buffer.h"
#include "mpu_prototypes.h"

#undef MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/*-----------------------------------------------------------*/

#if ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
    BaseType_t MPU_xTaskCreateRestricted( const TaskParameters_t * const pxTaskDefinition,
                                          TaskHandle_t * pxCreatedTask ) /* FREERTOS_SYSTEM_CALL */
    {
        BaseType_t xReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        xReturn = xTaskCreateRestricted( pxTaskDefinition, pxCreatedTask );
        vPortResetPrivilege(ulReturnAddress);
        return xReturn;
    }
#endif /* conifgSUPPORT_DYNAMIC_ALLOCATION */
/*-----------------------------------------------------------*/

#if ( configSUPPORT_STATIC_ALLOCATION == 1 )
    BaseType_t MPU_xTaskCreateRestrictedStatic( const TaskParameters_t * const pxTaskDefinition,
                                                TaskHandle_t * pxCreatedTask ) /* FREERTOS_SYSTEM_CALL */
    {
        BaseType_t xReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        xReturn = xTaskCreateRestrictedStatic( pxTaskDefinition, pxCreatedTask );
        vPortResetPrivilege(ulReturnAddress);
        return xReturn;
    }
#endif /* conifgSUPPORT_DYNAMIC_ALLOCATION */
/*-----------------------------------------------------------*/

#if ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
    BaseType_t MPU_xTaskCreate( TaskFunction_t pvTaskCode,
                                const char * const pcName,
                                uint16_t usStackDepth,
                                void * pvParameters,
                                UBaseType_t uxPriority,
                                TaskHandle_t * pxCreatedTask ) /* FREERTOS_SYSTEM_CALL */
    {
        BaseType_t xReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        xReturn = xTaskCreate( pvTaskCode, pcName, usStackDepth, pvParameters, uxPriority, pxCreatedTask );
        vPortResetPrivilege(ulReturnAddress);
        return xReturn;
    }
#endif /* configSUPPORT_DYNAMIC_ALLOCATION */
/*-----------------------------------------------------------*/

#if ( configSUPPORT_STATIC_ALLOCATION == 1 )
    TaskHandle_t MPU_xTaskCreateStatic( TaskFunction_t pxTaskCode,
                                        const char * const pcName,
                                        const uint32_t ulStackDepth,
                                        void * const pvParameters,
                                        UBaseType_t uxPriority,
                                        StackType_t * const puxStackBuffer,
                                        StaticTask_t * const pxTaskBuffer ) /* FREERTOS_SYSTEM_CALL */
    {
        TaskHandle_t xReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        xReturn = xTaskCreateStatic( pxTaskCode, pcName, ulStackDepth, pvParameters, uxPriority, puxStackBuffer, pxTaskBuffer );
        vPortResetPrivilege(ulReturnAddress);
        return xReturn;
    }
#endif /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

void MPU_vTaskAllocateMPURegions( TaskHandle_t xTask,
                                  const MemoryRegion_t * const xRegions ) /* FREERTOS_SYSTEM_CALL */
{
    UBaseType_t ulReturnAddress = xPortRaisePrivilege();

    vTaskAllocateMPURegions( xTask, xRegions );
    vPortResetPrivilege(ulReturnAddress);
}
/*-----------------------------------------------------------*/

#if ( INCLUDE_vTaskDelete == 1 )
    void MPU_vTaskDelete( TaskHandle_t pxTaskToDelete ) /* FREERTOS_SYSTEM_CALL */
    {
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        vTaskDelete( pxTaskToDelete );
        vPortResetPrivilege(ulReturnAddress);
    }
#endif
/*-----------------------------------------------------------*/

#if ( INCLUDE_vTaskDelayUntil == 1 )
    void MPU_vTaskDelayUntil( TickType_t * const pxPreviousWakeTime,
                              TickType_t xTimeIncrement ) /* FREERTOS_SYSTEM_CALL */
    {
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        vTaskDelayUntil( pxPreviousWakeTime, xTimeIncrement );
        vPortResetPrivilege(ulReturnAddress);
    }
#endif
/*-----------------------------------------------------------*/

#if ( INCLUDE_xTaskAbortDelay == 1 )
    BaseType_t MPU_xTaskAbortDelay( TaskHandle_t xTask ) /* FREERTOS_SYSTEM_CALL */
    {
        BaseType_t xReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        xReturn = xTaskAbortDelay( xTask );
        vPortResetPrivilege(ulReturnAddress);
        return xReturn;
    }
#endif
/*-----------------------------------------------------------*/

#if ( INCLUDE_vTaskDelay == 1 )
    void MPU_vTaskDelay( TickType_t xTicksToDelay ) /* FREERTOS_SYSTEM_CALL */
    {
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        vTaskDelay( xTicksToDelay );
        vPortResetPrivilege(ulReturnAddress);
    }
#endif
/*-----------------------------------------------------------*/

#if ( INCLUDE_uxTaskPriorityGet == 1 )
    UBaseType_t MPU_uxTaskPriorityGet( const TaskHandle_t pxTask ) /* FREERTOS_SYSTEM_CALL */
    {
        UBaseType_t uxReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        uxReturn = uxTaskPriorityGet( pxTask );
        vPortResetPrivilege(ulReturnAddress);
        return uxReturn;
    }
#endif
/*-----------------------------------------------------------*/

#if ( INCLUDE_vTaskPrioritySet == 1 )
    void MPU_vTaskPrioritySet( TaskHandle_t pxTask,
                               UBaseType_t uxNewPriority ) /* FREERTOS_SYSTEM_CALL */
    {
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        vTaskPrioritySet( pxTask, uxNewPriority );
        vPortResetPrivilege(ulReturnAddress);
    }
#endif
/*-----------------------------------------------------------*/

#if ( INCLUDE_eTaskGetState == 1 )
    eTaskState MPU_eTaskGetState( TaskHandle_t pxTask ) /* FREERTOS_SYSTEM_CALL */
    {
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();
        eTaskState eReturn;

        eReturn = eTaskGetState( pxTask );
        vPortResetPrivilege(ulReturnAddress);
        return eReturn;
    }
#endif
/*-----------------------------------------------------------*/

#if ( configUSE_TRACE_FACILITY == 1 )
    void MPU_vTaskGetInfo( TaskHandle_t xTask,
                           TaskStatus_t * pxTaskStatus,
                           BaseType_t xGetFreeStackSpace,
                           eTaskState eState ) /* FREERTOS_SYSTEM_CALL */
    {
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        vTaskGetInfo( xTask, pxTaskStatus, xGetFreeStackSpace, eState );
        vPortResetPrivilege(ulReturnAddress);
    }
#endif /* if ( configUSE_TRACE_FACILITY == 1 ) */
/*-----------------------------------------------------------*/

#if ( INCLUDE_xTaskGetIdleTaskHandle == 1 )
    TaskHandle_t MPU_xTaskGetIdleTaskHandle( void ) /* FREERTOS_SYSTEM_CALL */
    {
        TaskHandle_t xReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        xReturn = xTaskGetIdleTaskHandle();
        vPortResetPrivilege(ulReturnAddress);
        return xReturn;
    }
#endif
/*-----------------------------------------------------------*/

#if ( INCLUDE_vTaskSuspend == 1 )
    void MPU_vTaskSuspend( TaskHandle_t pxTaskToSuspend ) /* FREERTOS_SYSTEM_CALL */
    {
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        vTaskSuspend( pxTaskToSuspend );
        vPortResetPrivilege(ulReturnAddress);
    }
#endif
/*-----------------------------------------------------------*/

#if ( INCLUDE_vTaskSuspend == 1 )
    void MPU_vTaskResume( TaskHandle_t pxTaskToResume ) /* FREERTOS_SYSTEM_CALL */
    {
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        vTaskResume( pxTaskToResume );
        vPortResetPrivilege(ulReturnAddress);
    }
#endif
/*-----------------------------------------------------------*/

void MPU_vTaskSuspendAll( void ) /* FREERTOS_SYSTEM_CALL */
{
    UBaseType_t ulReturnAddress = xPortRaisePrivilege();

    vTaskSuspendAll();
    vPortResetPrivilege(ulReturnAddress);
}
/*-----------------------------------------------------------*/

BaseType_t MPU_xTaskResumeAll( void ) /* FREERTOS_SYSTEM_CALL */
{
    BaseType_t xReturn;
    UBaseType_t ulReturnAddress = xPortRaisePrivilege();

    xReturn = xTaskResumeAll();
    vPortResetPrivilege(ulReturnAddress);
    return xReturn;
}
/*-----------------------------------------------------------*/

TickType_t MPU_xTaskGetTickCount( void ) /* FREERTOS_SYSTEM_CALL */
{
    TickType_t xReturn;
    UBaseType_t ulReturnAddress = xPortRaisePrivilege();

    xReturn = xTaskGetTickCount();
    vPortResetPrivilege(ulReturnAddress);
    return xReturn;
}
/*-----------------------------------------------------------*/

UBaseType_t MPU_uxTaskGetNumberOfTasks( void ) /* FREERTOS_SYSTEM_CALL */
{
    UBaseType_t uxReturn;
    UBaseType_t ulReturnAddress = xPortRaisePrivilege();

    uxReturn = uxTaskGetNumberOfTasks();
    vPortResetPrivilege(ulReturnAddress);
    return uxReturn;
}
/*-----------------------------------------------------------*/

char * MPU_pcTaskGetName( TaskHandle_t xTaskToQuery ) /* FREERTOS_SYSTEM_CALL */
{
    char * pcReturn;
    UBaseType_t ulReturnAddress = xPortRaisePrivilege();

    pcReturn = pcTaskGetName( xTaskToQuery );
    vPortResetPrivilege(ulReturnAddress);
    return pcReturn;
}
/*-----------------------------------------------------------*/

#if ( INCLUDE_xTaskGetHandle == 1 )
    TaskHandle_t MPU_xTaskGetHandle( const char * pcNameToQuery ) /* FREERTOS_SYSTEM_CALL */
    {
        TaskHandle_t xReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        xReturn = xTaskGetHandle( pcNameToQuery );
        vPortResetPrivilege(ulReturnAddress);
        return xReturn;
    }
#endif
/*-----------------------------------------------------------*/

#if ( ( configUSE_TRACE_FACILITY == 1 ) && ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) )
    void MPU_vTaskList( char * pcWriteBuffer ) /* FREERTOS_SYSTEM_CALL */
    {
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        vTaskList( pcWriteBuffer );
        vPortResetPrivilege(ulReturnAddress);
    }
#endif
/*-----------------------------------------------------------*/

#if ( ( configGENERATE_RUN_TIME_STATS == 1 ) && ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) )
    void MPU_vTaskGetRunTimeStats( char * pcWriteBuffer ) /* FREERTOS_SYSTEM_CALL */
    {
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        vTaskGetRunTimeStats( pcWriteBuffer );
        vPortResetPrivilege(ulReturnAddress);
    }
#endif
/*-----------------------------------------------------------*/

#if ( ( configGENERATE_RUN_TIME_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) )
    uint32_t MPU_ulTaskGetIdleRunTimeCounter( void ) /* FREERTOS_SYSTEM_CALL */
    {
        uint32_t xReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        xReturn = ulTaskGetIdleRunTimeCounter();
        vPortResetPrivilege(ulReturnAddress);
        return xReturn;
    }
#endif
/*-----------------------------------------------------------*/

#if ( configUSE_APPLICATION_TASK_TAG == 1 )
    void MPU_vTaskSetApplicationTaskTag( TaskHandle_t xTask,
                                         TaskHookFunction_t pxTagValue ) /* FREERTOS_SYSTEM_CALL */
    {
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        vTaskSetApplicationTaskTag( xTask, pxTagValue );
        vPortResetPrivilege(ulReturnAddress);
    }
#endif
/*-----------------------------------------------------------*/

#if ( configUSE_APPLICATION_TASK_TAG == 1 )
    TaskHookFunction_t MPU_xTaskGetApplicationTaskTag( TaskHandle_t xTask ) /* FREERTOS_SYSTEM_CALL */
    {
        TaskHookFunction_t xReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        xReturn = xTaskGetApplicationTaskTag( xTask );
        vPortResetPrivilege(ulReturnAddress);
        return xReturn;
    }
#endif
/*-----------------------------------------------------------*/

#if ( configNUM_THREAD_LOCAL_STORAGE_POINTERS != 0 )
    void MPU_vTaskSetThreadLocalStoragePointer( TaskHandle_t xTaskToSet,
                                                BaseType_t xIndex,
                                                void * pvValue ) /* FREERTOS_SYSTEM_CALL */
    {
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        vTaskSetThreadLocalStoragePointer( xTaskToSet, xIndex, pvValue );
        vPortResetPrivilege(ulReturnAddress);
    }
#endif
/*-----------------------------------------------------------*/

#if ( configNUM_THREAD_LOCAL_STORAGE_POINTERS != 0 )
    void * MPU_pvTaskGetThreadLocalStoragePointer( TaskHandle_t xTaskToQuery,
                                                   BaseType_t xIndex ) /* FREERTOS_SYSTEM_CALL */
    {
        void * pvReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        pvReturn = pvTaskGetThreadLocalStoragePointer( xTaskToQuery, xIndex );
        vPortResetPrivilege(ulReturnAddress);
        return pvReturn;
    }
#endif /* if ( configNUM_THREAD_LOCAL_STORAGE_POINTERS != 0 ) */
/*-----------------------------------------------------------*/

#if ( configUSE_APPLICATION_TASK_TAG == 1 )
    BaseType_t MPU_xTaskCallApplicationTaskHook( TaskHandle_t xTask,
                                                 void * pvParameter ) /* FREERTOS_SYSTEM_CALL */
    {
        BaseType_t xReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        xReturn = xTaskCallApplicationTaskHook( xTask, pvParameter );
        vPortResetPrivilege(ulReturnAddress);
        return xReturn;
    }
#endif /* if ( configUSE_APPLICATION_TASK_TAG == 1 ) */
/*-----------------------------------------------------------*/

#if ( configUSE_TRACE_FACILITY == 1 )
    UBaseType_t MPU_uxTaskGetSystemState( TaskStatus_t * pxTaskStatusArray,
                                          UBaseType_t uxArraySize,
                                          uint32_t * pulTotalRunTime ) /* FREERTOS_SYSTEM_CALL */
    {
        UBaseType_t uxReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        uxReturn = uxTaskGetSystemState( pxTaskStatusArray, uxArraySize, pulTotalRunTime );
        vPortResetPrivilege(ulReturnAddress);
        return uxReturn;
    }
#endif /* if ( configUSE_TRACE_FACILITY == 1 ) */
/*-----------------------------------------------------------*/

BaseType_t MPU_xTaskCatchUpTicks( TickType_t xTicksToCatchUp ) /* FREERTOS_SYSTEM_CALL */
{
    BaseType_t xReturn;
    UBaseType_t ulReturnAddress = xPortRaisePrivilege();

    xReturn = xTaskCatchUpTicks( xTicksToCatchUp );
    vPortResetPrivilege(ulReturnAddress);
    return xReturn;
}
/*-----------------------------------------------------------*/

#if ( INCLUDE_uxTaskGetStackHighWaterMark == 1 )
    UBaseType_t MPU_uxTaskGetStackHighWaterMark( TaskHandle_t xTask ) /* FREERTOS_SYSTEM_CALL */
    {
        UBaseType_t uxReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        uxReturn = uxTaskGetStackHighWaterMark( xTask );
        vPortResetPrivilege(ulReturnAddress);
        return uxReturn;
    }
#endif
/*-----------------------------------------------------------*/

#if ( INCLUDE_uxTaskGetStackHighWaterMark2 == 1 )
    configSTACK_DEPTH_TYPE MPU_uxTaskGetStackHighWaterMark2( TaskHandle_t xTask ) /* FREERTOS_SYSTEM_CALL */
    {
        configSTACK_DEPTH_TYPE uxReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        uxReturn = uxTaskGetStackHighWaterMark2( xTask );
        vPortResetPrivilege(ulReturnAddress);
        return uxReturn;
    }
#endif
/*-----------------------------------------------------------*/

#if ( INCLUDE_xTaskGetCurrentTaskHandle == 1 )
    TaskHandle_t MPU_xTaskGetCurrentTaskHandle( void ) /* FREERTOS_SYSTEM_CALL */
    {
        TaskHandle_t xReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        xReturn = xTaskGetCurrentTaskHandle();
        vPortResetPrivilege(ulReturnAddress);
        return xReturn;
    }
#endif
/*-----------------------------------------------------------*/

#if ( INCLUDE_xTaskGetSchedulerState == 1 )
    BaseType_t MPU_xTaskGetSchedulerState( void ) /* FREERTOS_SYSTEM_CALL */
    {
        BaseType_t xReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        xReturn = xTaskGetSchedulerState();
        vPortResetPrivilege(ulReturnAddress);
        return xReturn;
    }
#endif
/*-----------------------------------------------------------*/

void MPU_vTaskSetTimeOutState( TimeOut_t * const pxTimeOut ) /* FREERTOS_SYSTEM_CALL */
{
    UBaseType_t ulReturnAddress = xPortRaisePrivilege();

    vTaskSetTimeOutState( pxTimeOut );
    vPortResetPrivilege(ulReturnAddress);
}
/*-----------------------------------------------------------*/

BaseType_t MPU_xTaskCheckForTimeOut( TimeOut_t * const pxTimeOut,
                                     TickType_t * const pxTicksToWait ) /* FREERTOS_SYSTEM_CALL */
{
    BaseType_t xReturn;
    UBaseType_t ulReturnAddress = xPortRaisePrivilege();

    xReturn = xTaskCheckForTimeOut( pxTimeOut, pxTicksToWait );
    vPortResetPrivilege(ulReturnAddress);
    return xReturn;
}
/*-----------------------------------------------------------*/

#if ( configUSE_TASK_NOTIFICATIONS == 1 )
    BaseType_t MPU_xTaskGenericNotify( TaskHandle_t xTaskToNotify,
                                       UBaseType_t uxIndexToNotify,
                                       uint32_t ulValue,
                                       eNotifyAction eAction,
                                       uint32_t * pulPreviousNotificationValue ) /* FREERTOS_SYSTEM_CALL */
    {
        BaseType_t xReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        xReturn = xTaskGenericNotify( xTaskToNotify, uxIndexToNotify, ulValue, eAction, pulPreviousNotificationValue );
        vPortResetPrivilege(ulReturnAddress);
        return xReturn;
    }
#endif /* if ( configUSE_TASK_NOTIFICATIONS == 1 ) */
/*-----------------------------------------------------------*/

#if ( configUSE_TASK_NOTIFICATIONS == 1 )
    BaseType_t MPU_xTaskGenericNotifyWait( UBaseType_t uxIndexToWaitOn,
                                           uint32_t ulBitsToClearOnEntry,
                                           uint32_t ulBitsToClearOnExit,
                                           uint32_t * pulNotificationValue,
                                           TickType_t xTicksToWait ) /* FREERTOS_SYSTEM_CALL */
    {
        BaseType_t xReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        xReturn = xTaskGenericNotifyWait( uxIndexToWaitOn, ulBitsToClearOnEntry, ulBitsToClearOnExit, pulNotificationValue, xTicksToWait );
        vPortResetPrivilege(ulReturnAddress);
        return xReturn;
    }
#endif /* if ( configUSE_TASK_NOTIFICATIONS == 1 ) */
/*-----------------------------------------------------------*/

#if ( configUSE_TASK_NOTIFICATIONS == 1 )
    uint32_t MPU_ulTaskGenericNotifyTake( UBaseType_t uxIndexToWaitOn,
                                          BaseType_t xClearCountOnExit,
                                          TickType_t xTicksToWait ) /* FREERTOS_SYSTEM_CALL */
    {
        uint32_t ulReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        ulReturn = ulTaskGenericNotifyTake( uxIndexToWaitOn, xClearCountOnExit, xTicksToWait );
        vPortResetPrivilege(ulReturnAddress);
        return ulReturn;
    }
#endif /* if ( configUSE_TASK_NOTIFICATIONS == 1 ) */
/*-----------------------------------------------------------*/

#if ( configUSE_TASK_NOTIFICATIONS == 1 )
    BaseType_t MPU_xTaskGenericNotifyStateClear( TaskHandle_t xTask,
                                                 UBaseType_t uxIndexToClear ) /* FREERTOS_SYSTEM_CALL */
    {
        BaseType_t xReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        xReturn = xTaskGenericNotifyStateClear( xTask, uxIndexToClear );
        vPortResetPrivilege(ulReturnAddress);
        return xReturn;
    }
#endif /* if ( configUSE_TASK_NOTIFICATIONS == 1 ) */
/*-----------------------------------------------------------*/

#if ( configUSE_TASK_NOTIFICATIONS == 1 )
    uint32_t MPU_ulTaskGenericNotifyValueClear( TaskHandle_t xTask,
                                                UBaseType_t uxIndexToClear,
                                                uint32_t ulBitsToClear ) /* FREERTOS_SYSTEM_CALL */
    {
        uint32_t ulReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        ulReturn = ulTaskGenericNotifyValueClear( xTask, uxIndexToClear, ulBitsToClear );
        vPortResetPrivilege(ulReturnAddress);
        return ulReturn;
    }
#endif /* if ( configUSE_TASK_NOTIFICATIONS == 1 ) */
/*-----------------------------------------------------------*/

#if ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
    QueueHandle_t MPU_xQueueGenericCreate( UBaseType_t uxQueueLength,
                                           UBaseType_t uxItemSize,
                                           uint8_t ucQueueType ) /* FREERTOS_SYSTEM_CALL */
    {
        QueueHandle_t xReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        xReturn = xQueueGenericCreate( uxQueueLength, uxItemSize, ucQueueType );

        vPortResetPrivilege(ulReturnAddress);

        return xReturn;
    }
#endif /* if ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) */
/*-----------------------------------------------------------*/

#if ( configSUPPORT_STATIC_ALLOCATION == 1 )
    QueueHandle_t MPU_xQueueGenericCreateStatic( const UBaseType_t uxQueueLength,
                                                 const UBaseType_t uxItemSize,
                                                 uint8_t * pucQueueStorage,
                                                 StaticQueue_t * pxStaticQueue,
                                                 const uint8_t ucQueueType ) /* FREERTOS_SYSTEM_CALL */
    {
        QueueHandle_t xReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        xReturn = xQueueGenericCreateStatic( uxQueueLength, uxItemSize, pucQueueStorage, pxStaticQueue, ucQueueType );
        vPortResetPrivilege(ulReturnAddress);
        return xReturn;
    }
#endif /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*-----------------------------------------------------------*/

BaseType_t MPU_xQueueGenericReset( QueueHandle_t pxQueue,
                                   BaseType_t xNewQueue ) /* FREERTOS_SYSTEM_CALL */
{
    BaseType_t xReturn;
    UBaseType_t ulReturnAddress = xPortRaisePrivilege();

    xReturn = xQueueGenericReset( pxQueue, xNewQueue );
    vPortResetPrivilege(ulReturnAddress);
    return xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t MPU_xQueueGenericSend( QueueHandle_t xQueue,
                                  const void * const pvItemToQueue,
                                  TickType_t xTicksToWait,
                                  BaseType_t xCopyPosition ) /* FREERTOS_SYSTEM_CALL */
{
    BaseType_t xReturn;
    UBaseType_t ulReturnAddress = xPortRaisePrivilege();

    xReturn = xQueueGenericSend( xQueue, pvItemToQueue, xTicksToWait, xCopyPosition );
    vPortResetPrivilege(ulReturnAddress);
    return xReturn;
}
/*-----------------------------------------------------------*/

UBaseType_t MPU_uxQueueMessagesWaiting( const QueueHandle_t pxQueue ) /* FREERTOS_SYSTEM_CALL */
{
    UBaseType_t ulReturnAddress = xPortRaisePrivilege();
    UBaseType_t uxReturn;

    uxReturn = uxQueueMessagesWaiting( pxQueue );
    vPortResetPrivilege(ulReturnAddress);
    return uxReturn;
}
/*-----------------------------------------------------------*/

UBaseType_t MPU_uxQueueSpacesAvailable( const QueueHandle_t xQueue ) /* FREERTOS_SYSTEM_CALL */
{
    UBaseType_t ulReturnAddress = xPortRaisePrivilege();
    UBaseType_t uxReturn;

    uxReturn = uxQueueSpacesAvailable( xQueue );
    vPortResetPrivilege(ulReturnAddress);
    return uxReturn;
}
/*-----------------------------------------------------------*/

BaseType_t MPU_xQueueReceive( QueueHandle_t pxQueue,
                              void * const pvBuffer,
                              TickType_t xTicksToWait ) /* FREERTOS_SYSTEM_CALL */
{
    UBaseType_t ulReturnAddress = xPortRaisePrivilege();
    BaseType_t xReturn;

    xReturn = xQueueReceive( pxQueue, pvBuffer, xTicksToWait );
    vPortResetPrivilege(ulReturnAddress);
    return xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t MPU_xQueuePeek( QueueHandle_t xQueue,
                           void * const pvBuffer,
                           TickType_t xTicksToWait ) /* FREERTOS_SYSTEM_CALL */
{
    UBaseType_t ulReturnAddress = xPortRaisePrivilege();
    BaseType_t xReturn;

    xReturn = xQueuePeek( xQueue, pvBuffer, xTicksToWait );
    vPortResetPrivilege(ulReturnAddress);
    return xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t MPU_xQueueSemaphoreTake( QueueHandle_t xQueue,
                                    TickType_t xTicksToWait ) /* FREERTOS_SYSTEM_CALL */
{
    UBaseType_t ulReturnAddress = xPortRaisePrivilege();
    BaseType_t xReturn;

    xReturn = xQueueSemaphoreTake( xQueue, xTicksToWait );
    vPortResetPrivilege(ulReturnAddress);
    return xReturn;
}
/*-----------------------------------------------------------*/

#if ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) )
    TaskHandle_t MPU_xQueueGetMutexHolder( QueueHandle_t xSemaphore ) /* FREERTOS_SYSTEM_CALL */
    {
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();
        void * xReturn;

        xReturn = xQueueGetMutexHolder( xSemaphore );
        vPortResetPrivilege(ulReturnAddress);
        return xReturn;
    }
#endif
/*-----------------------------------------------------------*/

#if ( ( configUSE_MUTEXES == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) )
    QueueHandle_t MPU_xQueueCreateMutex( const uint8_t ucQueueType ) /* FREERTOS_SYSTEM_CALL */
    {
        QueueHandle_t xReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        xReturn = xQueueCreateMutex( ucQueueType );
        vPortResetPrivilege(ulReturnAddress);
        return xReturn;
    }
#endif
/*-----------------------------------------------------------*/

#if ( ( configUSE_MUTEXES == 1 ) && ( configSUPPORT_STATIC_ALLOCATION == 1 ) )
    QueueHandle_t MPU_xQueueCreateMutexStatic( const uint8_t ucQueueType,
                                               StaticQueue_t * pxStaticQueue ) /* FREERTOS_SYSTEM_CALL */
    {
        QueueHandle_t xReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        xReturn = xQueueCreateMutexStatic( ucQueueType, pxStaticQueue );
        vPortResetPrivilege(ulReturnAddress);
        return xReturn;
    }
#endif /* if ( ( configUSE_MUTEXES == 1 ) && ( configSUPPORT_STATIC_ALLOCATION == 1 ) ) */
/*-----------------------------------------------------------*/

#if ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) )
    QueueHandle_t MPU_xQueueCreateCountingSemaphore( UBaseType_t uxCountValue,
                                                     UBaseType_t uxInitialCount ) /* FREERTOS_SYSTEM_CALL */
    {
        QueueHandle_t xReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        xReturn = xQueueCreateCountingSemaphore( uxCountValue, uxInitialCount );
        vPortResetPrivilege(ulReturnAddress);
        return xReturn;
    }
#endif /* if ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) ) */
/*-----------------------------------------------------------*/

#if ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( configSUPPORT_STATIC_ALLOCATION == 1 ) )

    QueueHandle_t MPU_xQueueCreateCountingSemaphoreStatic( const UBaseType_t uxMaxCount,
                                                           const UBaseType_t uxInitialCount,
                                                           StaticQueue_t * pxStaticQueue ) /* FREERTOS_SYSTEM_CALL */
    {
        QueueHandle_t xReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        xReturn = xQueueCreateCountingSemaphoreStatic( uxMaxCount, uxInitialCount, pxStaticQueue );
        vPortResetPrivilege(ulReturnAddress);
        return xReturn;
    }
#endif /* if ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( configSUPPORT_STATIC_ALLOCATION == 1 ) ) */
/*-----------------------------------------------------------*/

#if ( configUSE_RECURSIVE_MUTEXES == 1 )
    BaseType_t MPU_xQueueTakeMutexRecursive( QueueHandle_t xMutex,
                                             TickType_t xBlockTime ) /* FREERTOS_SYSTEM_CALL */
    {
        BaseType_t xReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        xReturn = xQueueTakeMutexRecursive( xMutex, xBlockTime );
        vPortResetPrivilege(ulReturnAddress);
        return xReturn;
    }
#endif /* if ( configUSE_RECURSIVE_MUTEXES == 1 ) */
/*-----------------------------------------------------------*/

#if ( configUSE_RECURSIVE_MUTEXES == 1 )
    BaseType_t MPU_xQueueGiveMutexRecursive( QueueHandle_t xMutex ) /* FREERTOS_SYSTEM_CALL */
    {
        BaseType_t xReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        xReturn = xQueueGiveMutexRecursive( xMutex );
        vPortResetPrivilege(ulReturnAddress);
        return xReturn;
    }
#endif
/*-----------------------------------------------------------*/

#if ( ( configUSE_QUEUE_SETS == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) )
    QueueSetHandle_t MPU_xQueueCreateSet( UBaseType_t uxEventQueueLength ) /* FREERTOS_SYSTEM_CALL */
    {
        QueueSetHandle_t xReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        xReturn = xQueueCreateSet( uxEventQueueLength );
        vPortResetPrivilege(ulReturnAddress);
        return xReturn;
    }
#endif
/*-----------------------------------------------------------*/

#if ( configUSE_QUEUE_SETS == 1 )
    QueueSetMemberHandle_t MPU_xQueueSelectFromSet( QueueSetHandle_t xQueueSet,
                                                    TickType_t xBlockTimeTicks ) /* FREERTOS_SYSTEM_CALL */
    {
        QueueSetMemberHandle_t xReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        xReturn = xQueueSelectFromSet( xQueueSet, xBlockTimeTicks );
        vPortResetPrivilege(ulReturnAddress);
        return xReturn;
    }
#endif /* if ( configUSE_QUEUE_SETS == 1 ) */
/*-----------------------------------------------------------*/

#if ( configUSE_QUEUE_SETS == 1 )
    BaseType_t MPU_xQueueAddToSet( QueueSetMemberHandle_t xQueueOrSemaphore,
                                   QueueSetHandle_t xQueueSet ) /* FREERTOS_SYSTEM_CALL */
    {
        BaseType_t xReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        xReturn = xQueueAddToSet( xQueueOrSemaphore, xQueueSet );
        vPortResetPrivilege(ulReturnAddress);
        return xReturn;
    }
#endif /* if ( configUSE_QUEUE_SETS == 1 ) */
/*-----------------------------------------------------------*/

#if ( configUSE_QUEUE_SETS == 1 )
    BaseType_t MPU_xQueueRemoveFromSet( QueueSetMemberHandle_t xQueueOrSemaphore,
                                        QueueSetHandle_t xQueueSet ) /* FREERTOS_SYSTEM_CALL */
    {
        BaseType_t xReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        xReturn = xQueueRemoveFromSet( xQueueOrSemaphore, xQueueSet );
        vPortResetPrivilege(ulReturnAddress);
        return xReturn;
    }
#endif /* if ( configUSE_QUEUE_SETS == 1 ) */
/*-----------------------------------------------------------*/

#if configQUEUE_REGISTRY_SIZE > 0
    void MPU_vQueueAddToRegistry( QueueHandle_t xQueue,
                                  const char * pcName ) /* FREERTOS_SYSTEM_CALL */
    {
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        vQueueAddToRegistry( xQueue, pcName );

        vPortResetPrivilege(ulReturnAddress);
    }
#endif
/*-----------------------------------------------------------*/

#if configQUEUE_REGISTRY_SIZE > 0
    void MPU_vQueueUnregisterQueue( QueueHandle_t xQueue ) /* FREERTOS_SYSTEM_CALL */
    {
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        vQueueUnregisterQueue( xQueue );

        vPortResetPrivilege(ulReturnAddress);
    }
#endif
/*-----------------------------------------------------------*/

#if configQUEUE_REGISTRY_SIZE > 0
    const char * MPU_pcQueueGetName( QueueHandle_t xQueue ) /* FREERTOS_SYSTEM_CALL */
    {
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();
        const char * pcReturn;

        pcReturn = pcQueueGetName( xQueue );

        vPortResetPrivilege(ulReturnAddress);
        return pcReturn;
    }
#endif /* if configQUEUE_REGISTRY_SIZE > 0 */
/*-----------------------------------------------------------*/

void MPU_vQueueDelete( QueueHandle_t xQueue ) /* FREERTOS_SYSTEM_CALL */
{
    UBaseType_t ulReturnAddress = xPortRaisePrivilege();

    vQueueDelete( xQueue );

    vPortResetPrivilege(ulReturnAddress);
}
/*-----------------------------------------------------------*/

#if ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
    void MPU_vPortInitialiseBlocks( void ) /* FREERTOS_SYSTEM_CALL */
    {
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        vPortInitialiseBlocks();

        vPortResetPrivilege(ulReturnAddress);
    }
#endif /* configSUPPORT_DYNAMIC_ALLOCATION */
/*-----------------------------------------------------------*/

#if ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
    size_t MPU_xPortGetFreeHeapSize( void ) /* FREERTOS_SYSTEM_CALL */
    {
        size_t xReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        xReturn = xPortGetFreeHeapSize();

        vPortResetPrivilege(ulReturnAddress);

        return xReturn;
    }
#endif /* configSUPPORT_DYNAMIC_ALLOCATION */
/*-----------------------------------------------------------*/

#if ( ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) && ( configUSE_TIMERS == 1 ) )
    TimerHandle_t MPU_xTimerCreate( const char * const pcTimerName,
                                    const TickType_t xTimerPeriodInTicks,
                                    const UBaseType_t uxAutoReload,
                                    void * const pvTimerID,
                                    TimerCallbackFunction_t pxCallbackFunction ) /* FREERTOS_SYSTEM_CALL */
    {
        TimerHandle_t xReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        xReturn = xTimerCreate( pcTimerName, xTimerPeriodInTicks, uxAutoReload, pvTimerID, pxCallbackFunction );
        vPortResetPrivilege(ulReturnAddress);

        return xReturn;
    }
#endif /* if ( ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) && ( configUSE_TIMERS == 1 ) ) */
/*-----------------------------------------------------------*/

#if ( ( configSUPPORT_STATIC_ALLOCATION == 1 ) && ( configUSE_TIMERS == 1 ) )
    TimerHandle_t MPU_xTimerCreateStatic( const char * const pcTimerName,
                                          const TickType_t xTimerPeriodInTicks,
                                          const UBaseType_t uxAutoReload,
                                          void * const pvTimerID,
                                          TimerCallbackFunction_t pxCallbackFunction,
                                          StaticTimer_t * pxTimerBuffer ) /* FREERTOS_SYSTEM_CALL */
    {
        TimerHandle_t xReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        xReturn = xTimerCreateStatic( pcTimerName, xTimerPeriodInTicks, uxAutoReload, pvTimerID, pxCallbackFunction, pxTimerBuffer );
        vPortResetPrivilege(ulReturnAddress);

        return xReturn;
    }
#endif /* if ( ( configSUPPORT_STATIC_ALLOCATION == 1 ) && ( configUSE_TIMERS == 1 ) ) */
/*-----------------------------------------------------------*/

#if ( configUSE_TIMERS == 1 )
    void * MPU_pvTimerGetTimerID( const TimerHandle_t xTimer ) /* FREERTOS_SYSTEM_CALL */
    {
        void * pvReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        pvReturn = pvTimerGetTimerID( xTimer );
        vPortResetPrivilege(ulReturnAddress);

        return pvReturn;
    }
#endif /* if ( configUSE_TIMERS == 1 ) */
/*-----------------------------------------------------------*/

#if ( configUSE_TIMERS == 1 )
    void MPU_vTimerSetTimerID( TimerHandle_t xTimer,
                               void * pvNewID ) /* FREERTOS_SYSTEM_CALL */
    {
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        vTimerSetTimerID( xTimer, pvNewID );
        vPortResetPrivilege(ulReturnAddress);
    }
#endif
/*-----------------------------------------------------------*/

#if ( configUSE_TIMERS == 1 )
    BaseType_t MPU_xTimerIsTimerActive( TimerHandle_t xTimer ) /* FREERTOS_SYSTEM_CALL */
    {
        BaseType_t xReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        xReturn = xTimerIsTimerActive( xTimer );
        vPortResetPrivilege(ulReturnAddress);

        return xReturn;
    }
#endif /* if ( configUSE_TIMERS == 1 ) */
/*-----------------------------------------------------------*/

#if ( configUSE_TIMERS == 1 )
    TaskHandle_t MPU_xTimerGetTimerDaemonTaskHandle( void ) /* FREERTOS_SYSTEM_CALL */
    {
        TaskHandle_t xReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        xReturn = xTimerGetTimerDaemonTaskHandle();
        vPortResetPrivilege(ulReturnAddress);

        return xReturn;
    }
#endif /* if ( configUSE_TIMERS == 1 ) */
/*-----------------------------------------------------------*/

#if ( ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 1 ) )
    BaseType_t MPU_xTimerPendFunctionCall( PendedFunction_t xFunctionToPend,
                                           void * pvParameter1,
                                           uint32_t ulParameter2,
                                           TickType_t xTicksToWait ) /* FREERTOS_SYSTEM_CALL */
    {
        BaseType_t xReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        xReturn = xTimerPendFunctionCall( xFunctionToPend, pvParameter1, ulParameter2, xTicksToWait );
        vPortResetPrivilege(ulReturnAddress);

        return xReturn;
    }
#endif /* if ( ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 1 ) ) */
/*-----------------------------------------------------------*/

#if ( configUSE_TIMERS == 1 )
    void MPU_vTimerSetReloadMode( TimerHandle_t xTimer,
                                  const UBaseType_t uxAutoReload ) /* FREERTOS_SYSTEM_CALL */
    {
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        vTimerSetReloadMode( xTimer, uxAutoReload );
        vPortResetPrivilege(ulReturnAddress);
    }
#endif
/*-----------------------------------------------------------*/

#if ( configUSE_TIMERS == 1 )
    UBaseType_t MPU_uxTimerGetReloadMode( TimerHandle_t xTimer )
    {
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();
        UBaseType_t uxReturn;

        uxReturn = uxTimerGetReloadMode( xTimer );
        vPortResetPrivilege(ulReturnAddress);
        return uxReturn;
    }
#endif
/*-----------------------------------------------------------*/

#if ( configUSE_TIMERS == 1 )
    const char * MPU_pcTimerGetName( TimerHandle_t xTimer ) /* FREERTOS_SYSTEM_CALL */
    {
        const char * pcReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        pcReturn = pcTimerGetName( xTimer );
        vPortResetPrivilege(ulReturnAddress);

        return pcReturn;
    }
#endif /* if ( configUSE_TIMERS == 1 ) */
/*-----------------------------------------------------------*/

#if ( configUSE_TIMERS == 1 )
    TickType_t MPU_xTimerGetPeriod( TimerHandle_t xTimer ) /* FREERTOS_SYSTEM_CALL */
    {
        TickType_t xReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        xReturn = xTimerGetPeriod( xTimer );
        vPortResetPrivilege(ulReturnAddress);

        return xReturn;
    }
#endif /* if ( configUSE_TIMERS == 1 ) */
/*-----------------------------------------------------------*/

#if ( configUSE_TIMERS == 1 )
    TickType_t MPU_xTimerGetExpiryTime( TimerHandle_t xTimer ) /* FREERTOS_SYSTEM_CALL */
    {
        TickType_t xReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        xReturn = xTimerGetExpiryTime( xTimer );
        vPortResetPrivilege(ulReturnAddress);

        return xReturn;
    }
#endif /* if ( configUSE_TIMERS == 1 ) */
/*-----------------------------------------------------------*/

#if ( configUSE_TIMERS == 1 )
    BaseType_t MPU_xTimerGenericCommand( TimerHandle_t xTimer,
                                         const BaseType_t xCommandID,
                                         const TickType_t xOptionalValue,
                                         BaseType_t * const pxHigherPriorityTaskWoken,
                                         const TickType_t xTicksToWait ) /* FREERTOS_SYSTEM_CALL */
    {
        BaseType_t xReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        xReturn = xTimerGenericCommand( xTimer, xCommandID, xOptionalValue, pxHigherPriorityTaskWoken, xTicksToWait );
        vPortResetPrivilege(ulReturnAddress);

        return xReturn;
    }
#endif /* if ( configUSE_TIMERS == 1 ) */
/*-----------------------------------------------------------*/

#if ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
    EventGroupHandle_t MPU_xEventGroupCreate( void ) /* FREERTOS_SYSTEM_CALL */
    {
        EventGroupHandle_t xReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        xReturn = xEventGroupCreate();
        vPortResetPrivilege(ulReturnAddress);

        return xReturn;
    }
#endif /* if ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) */
/*-----------------------------------------------------------*/

#if ( configSUPPORT_STATIC_ALLOCATION == 1 )
    EventGroupHandle_t MPU_xEventGroupCreateStatic( StaticEventGroup_t * pxEventGroupBuffer ) /* FREERTOS_SYSTEM_CALL */
    {
        EventGroupHandle_t xReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        xReturn = xEventGroupCreateStatic( pxEventGroupBuffer );
        vPortResetPrivilege(ulReturnAddress);

        return xReturn;
    }
#endif /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*-----------------------------------------------------------*/

EventBits_t MPU_xEventGroupWaitBits( EventGroupHandle_t xEventGroup,
                                     const EventBits_t uxBitsToWaitFor,
                                     const BaseType_t xClearOnExit,
                                     const BaseType_t xWaitForAllBits,
                                     TickType_t xTicksToWait ) /* FREERTOS_SYSTEM_CALL */
{
    EventBits_t xReturn;
    UBaseType_t ulReturnAddress = xPortRaisePrivilege();

    xReturn = xEventGroupWaitBits( xEventGroup, uxBitsToWaitFor, xClearOnExit, xWaitForAllBits, xTicksToWait );
    vPortResetPrivilege(ulReturnAddress);

    return xReturn;
}
/*-----------------------------------------------------------*/

EventBits_t MPU_xEventGroupClearBits( EventGroupHandle_t xEventGroup,
                                      const EventBits_t uxBitsToClear ) /* FREERTOS_SYSTEM_CALL */
{
    EventBits_t xReturn;
    UBaseType_t ulReturnAddress = xPortRaisePrivilege();

    xReturn = xEventGroupClearBits( xEventGroup, uxBitsToClear );
    vPortResetPrivilege(ulReturnAddress);

    return xReturn;
}
/*-----------------------------------------------------------*/

EventBits_t MPU_xEventGroupSetBits( EventGroupHandle_t xEventGroup,
                                    const EventBits_t uxBitsToSet ) /* FREERTOS_SYSTEM_CALL */
{
    EventBits_t xReturn;
    UBaseType_t ulReturnAddress = xPortRaisePrivilege();

    xReturn = xEventGroupSetBits( xEventGroup, uxBitsToSet );
    vPortResetPrivilege(ulReturnAddress);

    return xReturn;
}
/*-----------------------------------------------------------*/

EventBits_t MPU_xEventGroupSync( EventGroupHandle_t xEventGroup,
                                 const EventBits_t uxBitsToSet,
                                 const EventBits_t uxBitsToWaitFor,
                                 TickType_t xTicksToWait ) /* FREERTOS_SYSTEM_CALL */
{
    EventBits_t xReturn;
    UBaseType_t ulReturnAddress = xPortRaisePrivilege();

    xReturn = xEventGroupSync( xEventGroup, uxBitsToSet, uxBitsToWaitFor, xTicksToWait );
    vPortResetPrivilege(ulReturnAddress);

    return xReturn;
}
/*-----------------------------------------------------------*/

void MPU_vEventGroupDelete( EventGroupHandle_t xEventGroup ) /* FREERTOS_SYSTEM_CALL */
{
    UBaseType_t ulReturnAddress = xPortRaisePrivilege();

    vEventGroupDelete( xEventGroup );
    vPortResetPrivilege(ulReturnAddress);
}
/*-----------------------------------------------------------*/

size_t MPU_xStreamBufferSend( StreamBufferHandle_t xStreamBuffer,
                              const void * pvTxData,
                              size_t xDataLengthBytes,
                              TickType_t xTicksToWait ) /* FREERTOS_SYSTEM_CALL */
{
    size_t xReturn;
    UBaseType_t ulReturnAddress = xPortRaisePrivilege();

    xReturn = xStreamBufferSend( xStreamBuffer, pvTxData, xDataLengthBytes, xTicksToWait );
    vPortResetPrivilege(ulReturnAddress);

    return xReturn;
}
/*-----------------------------------------------------------*/

size_t MPU_xStreamBufferNextMessageLengthBytes( StreamBufferHandle_t xStreamBuffer ) /* FREERTOS_SYSTEM_CALL */
{
    size_t xReturn;
    UBaseType_t ulReturnAddress = xPortRaisePrivilege();

    xReturn = xStreamBufferNextMessageLengthBytes( xStreamBuffer );
    vPortResetPrivilege(ulReturnAddress);

    return xReturn;
}
/*-----------------------------------------------------------*/

size_t MPU_xStreamBufferReceive( StreamBufferHandle_t xStreamBuffer,
                                 void * pvRxData,
                                 size_t xBufferLengthBytes,
                                 TickType_t xTicksToWait ) /* FREERTOS_SYSTEM_CALL */
{
    size_t xReturn;
    UBaseType_t ulReturnAddress = xPortRaisePrivilege();

    xReturn = xStreamBufferReceive( xStreamBuffer, pvRxData, xBufferLengthBytes, xTicksToWait );
    vPortResetPrivilege(ulReturnAddress);

    return xReturn;
}
/*-----------------------------------------------------------*/

void MPU_vStreamBufferDelete( StreamBufferHandle_t xStreamBuffer ) /* FREERTOS_SYSTEM_CALL */
{
    UBaseType_t ulReturnAddress = xPortRaisePrivilege();

    vStreamBufferDelete( xStreamBuffer );
    vPortResetPrivilege(ulReturnAddress);
}
/*-----------------------------------------------------------*/

BaseType_t MPU_xStreamBufferIsFull( StreamBufferHandle_t xStreamBuffer ) /* FREERTOS_SYSTEM_CALL */
{
    BaseType_t xReturn;
    UBaseType_t ulReturnAddress = xPortRaisePrivilege();

    xReturn = xStreamBufferIsFull( xStreamBuffer );
    vPortResetPrivilege(ulReturnAddress);

    return xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t MPU_xStreamBufferIsEmpty( StreamBufferHandle_t xStreamBuffer ) /* FREERTOS_SYSTEM_CALL */
{
    BaseType_t xReturn;
    UBaseType_t ulReturnAddress = xPortRaisePrivilege();

    xReturn = xStreamBufferIsEmpty( xStreamBuffer );
    vPortResetPrivilege(ulReturnAddress);

    return xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t MPU_xStreamBufferReset( StreamBufferHandle_t xStreamBuffer ) /* FREERTOS_SYSTEM_CALL */
{
    BaseType_t xReturn;
    UBaseType_t ulReturnAddress = xPortRaisePrivilege();

    xReturn = xStreamBufferReset( xStreamBuffer );
    vPortResetPrivilege(ulReturnAddress);

    return xReturn;
}
/*-----------------------------------------------------------*/

size_t MPU_xStreamBufferSpacesAvailable( StreamBufferHandle_t xStreamBuffer ) /* FREERTOS_SYSTEM_CALL */
{
    size_t xReturn;
    UBaseType_t ulReturnAddress = xPortRaisePrivilege();

    xReturn = xStreamBufferSpacesAvailable( xStreamBuffer );
    vPortResetPrivilege(ulReturnAddress);

    return xReturn;
}
/*-----------------------------------------------------------*/

size_t MPU_xStreamBufferBytesAvailable( StreamBufferHandle_t xStreamBuffer ) /* FREERTOS_SYSTEM_CALL */
{
    size_t xReturn;
    UBaseType_t ulReturnAddress = xPortRaisePrivilege();

    xReturn = xStreamBufferBytesAvailable( xStreamBuffer );
    vPortResetPrivilege(ulReturnAddress);

    return xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t MPU_xStreamBufferSetTriggerLevel( StreamBufferHandle_t xStreamBuffer,
                                             size_t xTriggerLevel ) /* FREERTOS_SYSTEM_CALL */
{
    BaseType_t xReturn;
    UBaseType_t ulReturnAddress = xPortRaisePrivilege();

    xReturn = xStreamBufferSetTriggerLevel( xStreamBuffer, xTriggerLevel );
    vPortResetPrivilege(ulReturnAddress);

    return xReturn;
}
/*-----------------------------------------------------------*/

#if ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
    StreamBufferHandle_t MPU_xStreamBufferGenericCreate( size_t xBufferSizeBytes,
                                                         size_t xTriggerLevelBytes,
                                                         BaseType_t xIsMessageBuffer ) /* FREERTOS_SYSTEM_CALL */
    {
        StreamBufferHandle_t xReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        xReturn = xStreamBufferGenericCreate( xBufferSizeBytes, xTriggerLevelBytes, xIsMessageBuffer );
        vPortResetPrivilege(ulReturnAddress);

        return xReturn;
    }
#endif /* configSUPPORT_DYNAMIC_ALLOCATION */
/*-----------------------------------------------------------*/

#if ( configSUPPORT_STATIC_ALLOCATION == 1 )
    StreamBufferHandle_t MPU_xStreamBufferGenericCreateStatic( size_t xBufferSizeBytes,
                                                               size_t xTriggerLevelBytes,
                                                               BaseType_t xIsMessageBuffer,
                                                               uint8_t * const pucStreamBufferStorageArea,
                                                               StaticStreamBuffer_t * const pxStaticStreamBuffer ) /* FREERTOS_SYSTEM_CALL */
    {
        StreamBufferHandle_t xReturn;
        UBaseType_t ulReturnAddress = xPortRaisePrivilege();

        xReturn = xStreamBufferGenericCreateStatic( xBufferSizeBytes, xTriggerLevelBytes, xIsMessageBuffer, pucStreamBufferStorageArea, pxStaticStreamBuffer );
        vPortResetPrivilege(ulReturnAddress);

        return xReturn;
    }
#endif /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/


/* Functions that the application writer wants to execute in privileged mode
 * can be defined in application_defined_privileged_functions.h.  The functions
 * must take the same format as those above whereby the privilege state on exit
 * equals the privilege state on entry.  For example:
 *
 * void MPU_FunctionName( [parameters ] )
 * {
 * UBaseType_t ulReturnAddress = xPortRaisePrivilege();
 *
 *  FunctionName( [parameters ] );
 *
 *  vPortResetPrivilege(ulReturnAddress);
 * }
 */

#if configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS == 1
    #include "application_defined_privileged_functions.h"
#endif
