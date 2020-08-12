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

/**
 * @brief Calls the port specific code to raise the privilege.
 *
 * @return pdFALSE if privilege was raised, pdTRUE otherwise.
 */
BaseType_t xPortRaisePrivilege( void ) FREERTOS_SYSTEM_CALL;

/**
 * @brief If xRunningPrivileged is not pdTRUE, calls the port specific
 * code to reset the privilege, otherwise does nothing.
 */
void vPortResetPrivilege( BaseType_t xRunningPrivileged );
/*-----------------------------------------------------------*/

BaseType_t xPortRaisePrivilege( void ) /* FREERTOS_SYSTEM_CALL */
{
    BaseType_t xRunningPrivileged;

    /* Check whether the processor is already privileged. */
    xRunningPrivileged = portIS_PRIVILEGED();

    /* If the processor is not already privileged, raise privilege. */
    if( xRunningPrivileged == pdFALSE )
    {
        portRAISE_PRIVILEGE();
    }

    return xRunningPrivileged;
}
/*-----------------------------------------------------------*/

void vPortResetPrivilege( BaseType_t xRunningPrivileged )
{
    if( xRunningPrivileged == pdFALSE )
    {
        portRESET_PRIVILEGE();
    }
}
/*-----------------------------------------------------------*/

#if ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
    BaseType_t MPU_xTaskCreateRestricted( const TaskParameters_t * const pxTaskDefinition,
                                          TaskHandle_t * pxCreatedTask ) /* FREERTOS_SYSTEM_CALL */
    {
        BaseType_t xReturn;
        xPortRaisePrivilege();

        xReturn = xTaskCreateRestricted( pxTaskDefinition, pxCreatedTask );
        vPortResetPrivilege();
        return xReturn;
    }
#endif /* conifgSUPPORT_DYNAMIC_ALLOCATION */
/*-----------------------------------------------------------*/

#if ( configSUPPORT_STATIC_ALLOCATION == 1 )
    BaseType_t MPU_xTaskCreateRestrictedStatic( const TaskParameters_t * const pxTaskDefinition,
                                                TaskHandle_t * pxCreatedTask ) /* FREERTOS_SYSTEM_CALL */
    {
        BaseType_t xReturn;
        xPortRaisePrivilege();

        xReturn = xTaskCreateRestrictedStatic( pxTaskDefinition, pxCreatedTask );
        vPortResetPrivilege();
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
        xPortRaisePrivilege();

        xReturn = xTaskCreate( pvTaskCode, pcName, usStackDepth, pvParameters, uxPriority, pxCreatedTask );
        vPortResetPrivilege();
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
        xPortRaisePrivilege();

        xReturn = xTaskCreateStatic( pxTaskCode, pcName, ulStackDepth, pvParameters, uxPriority, puxStackBuffer, pxTaskBuffer );
        vPortResetPrivilege();
        return xReturn;
    }
#endif /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

void MPU_vTaskAllocateMPURegions( TaskHandle_t xTask,
                                  const MemoryRegion_t * const xRegions ) /* FREERTOS_SYSTEM_CALL */
{
    xPortRaisePrivilege();

    vTaskAllocateMPURegions( xTask, xRegions );
    vPortResetPrivilege();
}
/*-----------------------------------------------------------*/

#if ( INCLUDE_vTaskDelete == 1 )
    void MPU_vTaskDelete( TaskHandle_t pxTaskToDelete ) /* FREERTOS_SYSTEM_CALL */
    {
        xPortRaisePrivilege();

        vTaskDelete( pxTaskToDelete );
        vPortResetPrivilege();
    }
#endif
/*-----------------------------------------------------------*/

#if ( INCLUDE_vTaskDelayUntil == 1 )
    void MPU_vTaskDelayUntil( TickType_t * const pxPreviousWakeTime,
                              TickType_t xTimeIncrement ) /* FREERTOS_SYSTEM_CALL */
    {
        xPortRaisePrivilege();

        vTaskDelayUntil( pxPreviousWakeTime, xTimeIncrement );
        vPortResetPrivilege();
    }
#endif
/*-----------------------------------------------------------*/

#if ( INCLUDE_xTaskAbortDelay == 1 )
    BaseType_t MPU_xTaskAbortDelay( TaskHandle_t xTask ) /* FREERTOS_SYSTEM_CALL */
    {
        BaseType_t xReturn;
        xPortRaisePrivilege();

        xReturn = xTaskAbortDelay( xTask );
        vPortResetPrivilege();
        return xReturn;
    }
#endif
/*-----------------------------------------------------------*/

#if ( INCLUDE_vTaskDelay == 1 )
    void MPU_vTaskDelay( TickType_t xTicksToDelay ) /* FREERTOS_SYSTEM_CALL */
    {
        xPortRaisePrivilege();

        vTaskDelay( xTicksToDelay );
        vPortResetPrivilege();
    }
#endif
/*-----------------------------------------------------------*/

#if ( INCLUDE_uxTaskPriorityGet == 1 )
    UBaseType_t MPU_uxTaskPriorityGet( const TaskHandle_t pxTask ) /* FREERTOS_SYSTEM_CALL */
    {
        UBaseType_t uxReturn;
        xPortRaisePrivilege();

        uxReturn = uxTaskPriorityGet( pxTask );
        vPortResetPrivilege();
        return uxReturn;
    }
#endif
/*-----------------------------------------------------------*/

#if ( INCLUDE_vTaskPrioritySet == 1 )
    void MPU_vTaskPrioritySet( TaskHandle_t pxTask,
                               UBaseType_t uxNewPriority ) /* FREERTOS_SYSTEM_CALL */
    {
        xPortRaisePrivilege();

        vTaskPrioritySet( pxTask, uxNewPriority );
        vPortResetPrivilege();
    }
#endif
/*-----------------------------------------------------------*/

#if ( INCLUDE_eTaskGetState == 1 )
    eTaskState MPU_eTaskGetState( TaskHandle_t pxTask ) /* FREERTOS_SYSTEM_CALL */
    {
        xPortRaisePrivilege();
        eTaskState eReturn;

        eReturn = eTaskGetState( pxTask );
        vPortResetPrivilege();
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
        xPortRaisePrivilege();

        vTaskGetInfo( xTask, pxTaskStatus, xGetFreeStackSpace, eState );
        vPortResetPrivilege();
    }
#endif /* if ( configUSE_TRACE_FACILITY == 1 ) */
/*-----------------------------------------------------------*/

#if ( INCLUDE_xTaskGetIdleTaskHandle == 1 )
    TaskHandle_t MPU_xTaskGetIdleTaskHandle( void ) /* FREERTOS_SYSTEM_CALL */
    {
        TaskHandle_t xReturn;
        xPortRaisePrivilege();

        xReturn = xTaskGetIdleTaskHandle();
        vPortResetPrivilege();
        return xReturn;
    }
#endif
/*-----------------------------------------------------------*/

#if ( INCLUDE_vTaskSuspend == 1 )
    void MPU_vTaskSuspend( TaskHandle_t pxTaskToSuspend ) /* FREERTOS_SYSTEM_CALL */
    {
        xPortRaisePrivilege();

        vTaskSuspend( pxTaskToSuspend );
        vPortResetPrivilege();
    }
#endif
/*-----------------------------------------------------------*/

#if ( INCLUDE_vTaskSuspend == 1 )
    void MPU_vTaskResume( TaskHandle_t pxTaskToResume ) /* FREERTOS_SYSTEM_CALL */
    {
        xPortRaisePrivilege();

        vTaskResume( pxTaskToResume );
        vPortResetPrivilege();
    }
#endif
/*-----------------------------------------------------------*/

void MPU_vTaskSuspendAll( void ) /* FREERTOS_SYSTEM_CALL */
{
    xPortRaisePrivilege();

    vTaskSuspendAll();
    vPortResetPrivilege();
}
/*-----------------------------------------------------------*/

BaseType_t MPU_xTaskResumeAll( void ) /* FREERTOS_SYSTEM_CALL */
{
    BaseType_t xReturn;
    xPortRaisePrivilege();

    xReturn = xTaskResumeAll();
    vPortResetPrivilege();
    return xReturn;
}
/*-----------------------------------------------------------*/

TickType_t MPU_xTaskGetTickCount( void ) /* FREERTOS_SYSTEM_CALL */
{
    TickType_t xReturn;
    xPortRaisePrivilege();

    xReturn = xTaskGetTickCount();
    vPortResetPrivilege();
    return xReturn;
}
/*-----------------------------------------------------------*/

UBaseType_t MPU_uxTaskGetNumberOfTasks( void ) /* FREERTOS_SYSTEM_CALL */
{
    UBaseType_t uxReturn;
    xPortRaisePrivilege();

    uxReturn = uxTaskGetNumberOfTasks();
    vPortResetPrivilege();
    return uxReturn;
}
/*-----------------------------------------------------------*/

char * MPU_pcTaskGetName( TaskHandle_t xTaskToQuery ) /* FREERTOS_SYSTEM_CALL */
{
    char * pcReturn;
    xPortRaisePrivilege();

    pcReturn = pcTaskGetName( xTaskToQuery );
    vPortResetPrivilege();
    return pcReturn;
}
/*-----------------------------------------------------------*/

#if ( INCLUDE_xTaskGetHandle == 1 )
    TaskHandle_t MPU_xTaskGetHandle( const char * pcNameToQuery ) /* FREERTOS_SYSTEM_CALL */
    {
        TaskHandle_t xReturn;
        xPortRaisePrivilege();

        xReturn = xTaskGetHandle( pcNameToQuery );
        vPortResetPrivilege();
        return xReturn;
    }
#endif
/*-----------------------------------------------------------*/

#if ( ( configUSE_TRACE_FACILITY == 1 ) && ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) )
    void MPU_vTaskList( char * pcWriteBuffer ) /* FREERTOS_SYSTEM_CALL */
    {
        xPortRaisePrivilege();

        vTaskList( pcWriteBuffer );
        vPortResetPrivilege();
    }
#endif
/*-----------------------------------------------------------*/

#if ( ( configGENERATE_RUN_TIME_STATS == 1 ) && ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) )
    void MPU_vTaskGetRunTimeStats( char * pcWriteBuffer ) /* FREERTOS_SYSTEM_CALL */
    {
        xPortRaisePrivilege();

        vTaskGetRunTimeStats( pcWriteBuffer );
        vPortResetPrivilege();
    }
#endif
/*-----------------------------------------------------------*/

#if ( ( configGENERATE_RUN_TIME_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) )
    uint32_t MPU_ulTaskGetIdleRunTimeCounter( void ) /* FREERTOS_SYSTEM_CALL */
    {
        uint32_t xReturn;
        xPortRaisePrivilege();

        xReturn = ulTaskGetIdleRunTimeCounter();
        vPortResetPrivilege();
        return xReturn;
    }
#endif
/*-----------------------------------------------------------*/

#if ( configUSE_APPLICATION_TASK_TAG == 1 )
    void MPU_vTaskSetApplicationTaskTag( TaskHandle_t xTask,
                                         TaskHookFunction_t pxTagValue ) /* FREERTOS_SYSTEM_CALL */
    {
        xPortRaisePrivilege();

        vTaskSetApplicationTaskTag( xTask, pxTagValue );
        vPortResetPrivilege();
    }
#endif
/*-----------------------------------------------------------*/

#if ( configUSE_APPLICATION_TASK_TAG == 1 )
    TaskHookFunction_t MPU_xTaskGetApplicationTaskTag( TaskHandle_t xTask ) /* FREERTOS_SYSTEM_CALL */
    {
        TaskHookFunction_t xReturn;
        xPortRaisePrivilege();

        xReturn = xTaskGetApplicationTaskTag( xTask );
        vPortResetPrivilege();
        return xReturn;
    }
#endif
/*-----------------------------------------------------------*/

#if ( configNUM_THREAD_LOCAL_STORAGE_POINTERS != 0 )
    void MPU_vTaskSetThreadLocalStoragePointer( TaskHandle_t xTaskToSet,
                                                BaseType_t xIndex,
                                                void * pvValue ) /* FREERTOS_SYSTEM_CALL */
    {
        xPortRaisePrivilege();

        vTaskSetThreadLocalStoragePointer( xTaskToSet, xIndex, pvValue );
        vPortResetPrivilege();
    }
#endif
/*-----------------------------------------------------------*/

#if ( configNUM_THREAD_LOCAL_STORAGE_POINTERS != 0 )
    void * MPU_pvTaskGetThreadLocalStoragePointer( TaskHandle_t xTaskToQuery,
                                                   BaseType_t xIndex ) /* FREERTOS_SYSTEM_CALL */
    {
        void * pvReturn;
        xPortRaisePrivilege();

        pvReturn = pvTaskGetThreadLocalStoragePointer( xTaskToQuery, xIndex );
        vPortResetPrivilege();
        return pvReturn;
    }
#endif /* if ( configNUM_THREAD_LOCAL_STORAGE_POINTERS != 0 ) */
/*-----------------------------------------------------------*/

#if ( configUSE_APPLICATION_TASK_TAG == 1 )
    BaseType_t MPU_xTaskCallApplicationTaskHook( TaskHandle_t xTask,
                                                 void * pvParameter ) /* FREERTOS_SYSTEM_CALL */
    {
        BaseType_t xReturn;
        xPortRaisePrivilege();

        xReturn = xTaskCallApplicationTaskHook( xTask, pvParameter );
        vPortResetPrivilege();
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
        xPortRaisePrivilege();

        uxReturn = uxTaskGetSystemState( pxTaskStatusArray, uxArraySize, pulTotalRunTime );
        vPortResetPrivilege();
        return uxReturn;
    }
#endif /* if ( configUSE_TRACE_FACILITY == 1 ) */
/*-----------------------------------------------------------*/

BaseType_t MPU_xTaskCatchUpTicks( TickType_t xTicksToCatchUp ) /* FREERTOS_SYSTEM_CALL */
{
    BaseType_t xReturn;
    xPortRaisePrivilege();

    xReturn = xTaskCatchUpTicks( xTicksToCatchUp );
    vPortResetPrivilege();
    return xReturn;
}
/*-----------------------------------------------------------*/

#if ( INCLUDE_uxTaskGetStackHighWaterMark == 1 )
    UBaseType_t MPU_uxTaskGetStackHighWaterMark( TaskHandle_t xTask ) /* FREERTOS_SYSTEM_CALL */
    {
        UBaseType_t uxReturn;
        xPortRaisePrivilege();

        uxReturn = uxTaskGetStackHighWaterMark( xTask );
        vPortResetPrivilege();
        return uxReturn;
    }
#endif
/*-----------------------------------------------------------*/

#if ( INCLUDE_uxTaskGetStackHighWaterMark2 == 1 )
    configSTACK_DEPTH_TYPE MPU_uxTaskGetStackHighWaterMark2( TaskHandle_t xTask ) /* FREERTOS_SYSTEM_CALL */
    {
        configSTACK_DEPTH_TYPE uxReturn;
        xPortRaisePrivilege();

        uxReturn = uxTaskGetStackHighWaterMark2( xTask );
        vPortResetPrivilege();
        return uxReturn;
    }
#endif
/*-----------------------------------------------------------*/

#if ( INCLUDE_xTaskGetCurrentTaskHandle == 1 )
    TaskHandle_t MPU_xTaskGetCurrentTaskHandle( void ) /* FREERTOS_SYSTEM_CALL */
    {
        TaskHandle_t xReturn;
        xPortRaisePrivilege();

        xReturn = xTaskGetCurrentTaskHandle();
        vPortResetPrivilege();
        return xReturn;
    }
#endif
/*-----------------------------------------------------------*/

#if ( INCLUDE_xTaskGetSchedulerState == 1 )
    BaseType_t MPU_xTaskGetSchedulerState( void ) /* FREERTOS_SYSTEM_CALL */
    {
        BaseType_t xReturn;
        xPortRaisePrivilege();

        xReturn = xTaskGetSchedulerState();
        vPortResetPrivilege();
        return xReturn;
    }
#endif
/*-----------------------------------------------------------*/

void MPU_vTaskSetTimeOutState( TimeOut_t * const pxTimeOut ) /* FREERTOS_SYSTEM_CALL */
{
    xPortRaisePrivilege();

    vTaskSetTimeOutState( pxTimeOut );
    vPortResetPrivilege();
}
/*-----------------------------------------------------------*/

BaseType_t MPU_xTaskCheckForTimeOut( TimeOut_t * const pxTimeOut,
                                     TickType_t * const pxTicksToWait ) /* FREERTOS_SYSTEM_CALL */
{
    BaseType_t xReturn;
    xPortRaisePrivilege();

    xReturn = xTaskCheckForTimeOut( pxTimeOut, pxTicksToWait );
    vPortResetPrivilege();
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
        xPortRaisePrivilege();

        xReturn = xTaskGenericNotify( xTaskToNotify, uxIndexToNotify, ulValue, eAction, pulPreviousNotificationValue );
        vPortResetPrivilege();
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
        xPortRaisePrivilege();

        xReturn = xTaskGenericNotifyWait( uxIndexToWaitOn, ulBitsToClearOnEntry, ulBitsToClearOnExit, pulNotificationValue, xTicksToWait );
        vPortResetPrivilege();
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
        xPortRaisePrivilege();

        ulReturn = ulTaskGenericNotifyTake( uxIndexToWaitOn, xClearCountOnExit, xTicksToWait );
        vPortResetPrivilege();
        return ulReturn;
    }
#endif /* if ( configUSE_TASK_NOTIFICATIONS == 1 ) */
/*-----------------------------------------------------------*/

#if ( configUSE_TASK_NOTIFICATIONS == 1 )
    BaseType_t MPU_xTaskGenericNotifyStateClear( TaskHandle_t xTask,
                                                 UBaseType_t uxIndexToClear ) /* FREERTOS_SYSTEM_CALL */
    {
        BaseType_t xReturn;
        xPortRaisePrivilege();

        xReturn = xTaskGenericNotifyStateClear( xTask, uxIndexToClear );
        vPortResetPrivilege();
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
        xPortRaisePrivilege();

        ulReturn = ulTaskGenericNotifyValueClear( xTask, uxIndexToClear, ulBitsToClear );
        vPortResetPrivilege();
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
        xPortRaisePrivilege();

        xReturn = xQueueGenericCreate( uxQueueLength, uxItemSize, ucQueueType );
        vPortResetPrivilege();
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
        xPortRaisePrivilege();

        xReturn = xQueueGenericCreateStatic( uxQueueLength, uxItemSize, pucQueueStorage, pxStaticQueue, ucQueueType );
        vPortResetPrivilege();
        return xReturn;
    }
#endif /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*-----------------------------------------------------------*/

BaseType_t MPU_xQueueGenericReset( QueueHandle_t pxQueue,
                                   BaseType_t xNewQueue ) /* FREERTOS_SYSTEM_CALL */
{
    BaseType_t xReturn;
    xPortRaisePrivilege();

    xReturn = xQueueGenericReset( pxQueue, xNewQueue );
    vPortResetPrivilege();
    return xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t MPU_xQueueGenericSend( QueueHandle_t xQueue,
                                  const void * const pvItemToQueue,
                                  TickType_t xTicksToWait,
                                  BaseType_t xCopyPosition ) /* FREERTOS_SYSTEM_CALL */
{
    BaseType_t xReturn;
    xPortRaisePrivilege();

    xReturn = xQueueGenericSend( xQueue, pvItemToQueue, xTicksToWait, xCopyPosition );
    vPortResetPrivilege();
    return xReturn;
}
/*-----------------------------------------------------------*/

UBaseType_t MPU_uxQueueMessagesWaiting( const QueueHandle_t pxQueue ) /* FREERTOS_SYSTEM_CALL */
{
    xPortRaisePrivilege();
    UBaseType_t uxReturn;

    uxReturn = uxQueueMessagesWaiting( pxQueue );
    vPortResetPrivilege();
    return uxReturn;
}
/*-----------------------------------------------------------*/

UBaseType_t MPU_uxQueueSpacesAvailable( const QueueHandle_t xQueue ) /* FREERTOS_SYSTEM_CALL */
{
    xPortRaisePrivilege();
    UBaseType_t uxReturn;

    uxReturn = uxQueueSpacesAvailable( xQueue );
    vPortResetPrivilege();
    return uxReturn;
}
/*-----------------------------------------------------------*/

BaseType_t MPU_xQueueReceive( QueueHandle_t pxQueue,
                              void * const pvBuffer,
                              TickType_t xTicksToWait ) /* FREERTOS_SYSTEM_CALL */
{
    xPortRaisePrivilege();
    BaseType_t xReturn;

    xReturn = xQueueReceive( pxQueue, pvBuffer, xTicksToWait );
    vPortResetPrivilege();
    return xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t MPU_xQueuePeek( QueueHandle_t xQueue,
                           void * const pvBuffer,
                           TickType_t xTicksToWait ) /* FREERTOS_SYSTEM_CALL */
{
    xPortRaisePrivilege();
    BaseType_t xReturn;

    xReturn = xQueuePeek( xQueue, pvBuffer, xTicksToWait );
    vPortResetPrivilege();
    return xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t MPU_xQueueSemaphoreTake( QueueHandle_t xQueue,
                                    TickType_t xTicksToWait ) /* FREERTOS_SYSTEM_CALL */
{
    xPortRaisePrivilege();
    BaseType_t xReturn;

    xReturn = xQueueSemaphoreTake( xQueue, xTicksToWait );
    vPortResetPrivilege();
    return xReturn;
}
/*-----------------------------------------------------------*/

#if ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) )
    TaskHandle_t MPU_xQueueGetMutexHolder( QueueHandle_t xSemaphore ) /* FREERTOS_SYSTEM_CALL */
    {
        xPortRaisePrivilege();
        void * xReturn;

        xReturn = xQueueGetMutexHolder( xSemaphore );
        vPortResetPrivilege();
        return xReturn;
    }
#endif
/*-----------------------------------------------------------*/

#if ( ( configUSE_MUTEXES == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) )
    QueueHandle_t MPU_xQueueCreateMutex( const uint8_t ucQueueType ) /* FREERTOS_SYSTEM_CALL */
    {
        QueueHandle_t xReturn;
        xPortRaisePrivilege();

        xReturn = xQueueCreateMutex( ucQueueType );
        vPortResetPrivilege();
        return xReturn;
    }
#endif
/*-----------------------------------------------------------*/

#if ( ( configUSE_MUTEXES == 1 ) && ( configSUPPORT_STATIC_ALLOCATION == 1 ) )
    QueueHandle_t MPU_xQueueCreateMutexStatic( const uint8_t ucQueueType,
                                               StaticQueue_t * pxStaticQueue ) /* FREERTOS_SYSTEM_CALL */
    {
        QueueHandle_t xReturn;
        xPortRaisePrivilege();

        xReturn = xQueueCreateMutexStatic( ucQueueType, pxStaticQueue );
        vPortResetPrivilege();
        return xReturn;
    }
#endif /* if ( ( configUSE_MUTEXES == 1 ) && ( configSUPPORT_STATIC_ALLOCATION == 1 ) ) */
/*-----------------------------------------------------------*/

#if ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) )
    QueueHandle_t MPU_xQueueCreateCountingSemaphore( UBaseType_t uxCountValue,
                                                     UBaseType_t uxInitialCount ) /* FREERTOS_SYSTEM_CALL */
    {
        QueueHandle_t xReturn;
        xPortRaisePrivilege();

        xReturn = xQueueCreateCountingSemaphore( uxCountValue, uxInitialCount );
        vPortResetPrivilege();
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
        xPortRaisePrivilege();

        xReturn = xQueueCreateCountingSemaphoreStatic( uxMaxCount, uxInitialCount, pxStaticQueue );
        vPortResetPrivilege();
        return xReturn;
    }
#endif /* if ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( configSUPPORT_STATIC_ALLOCATION == 1 ) ) */
/*-----------------------------------------------------------*/

#if ( configUSE_RECURSIVE_MUTEXES == 1 )
    BaseType_t MPU_xQueueTakeMutexRecursive( QueueHandle_t xMutex,
                                             TickType_t xBlockTime ) /* FREERTOS_SYSTEM_CALL */
    {
        BaseType_t xReturn;
        xPortRaisePrivilege();

        xReturn = xQueueTakeMutexRecursive( xMutex, xBlockTime );
        vPortResetPrivilege();
        return xReturn;
    }
#endif /* if ( configUSE_RECURSIVE_MUTEXES == 1 ) */
/*-----------------------------------------------------------*/

#if ( configUSE_RECURSIVE_MUTEXES == 1 )
    BaseType_t MPU_xQueueGiveMutexRecursive( QueueHandle_t xMutex ) /* FREERTOS_SYSTEM_CALL */
    {
        BaseType_t xReturn;
        xPortRaisePrivilege();

        xReturn = xQueueGiveMutexRecursive( xMutex );
        vPortResetPrivilege();
        return xReturn;
    }
#endif
/*-----------------------------------------------------------*/

#if ( ( configUSE_QUEUE_SETS == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) )
    QueueSetHandle_t MPU_xQueueCreateSet( UBaseType_t uxEventQueueLength ) /* FREERTOS_SYSTEM_CALL */
    {
        QueueSetHandle_t xReturn;
        xPortRaisePrivilege();

        xReturn = xQueueCreateSet( uxEventQueueLength );
        vPortResetPrivilege();
        return xReturn;
    }
#endif
/*-----------------------------------------------------------*/

#if ( configUSE_QUEUE_SETS == 1 )
    QueueSetMemberHandle_t MPU_xQueueSelectFromSet( QueueSetHandle_t xQueueSet,
                                                    TickType_t xBlockTimeTicks ) /* FREERTOS_SYSTEM_CALL */
    {
        QueueSetMemberHandle_t xReturn;
        xPortRaisePrivilege();

        xReturn = xQueueSelectFromSet( xQueueSet, xBlockTimeTicks );
        vPortResetPrivilege();
        return xReturn;
    }
#endif /* if ( configUSE_QUEUE_SETS == 1 ) */
/*-----------------------------------------------------------*/

#if ( configUSE_QUEUE_SETS == 1 )
    BaseType_t MPU_xQueueAddToSet( QueueSetMemberHandle_t xQueueOrSemaphore,
                                   QueueSetHandle_t xQueueSet ) /* FREERTOS_SYSTEM_CALL */
    {
        BaseType_t xReturn;
        xPortRaisePrivilege();

        xReturn = xQueueAddToSet( xQueueOrSemaphore, xQueueSet );
        vPortResetPrivilege();
        return xReturn;
    }
#endif /* if ( configUSE_QUEUE_SETS == 1 ) */
/*-----------------------------------------------------------*/

#if ( configUSE_QUEUE_SETS == 1 )
    BaseType_t MPU_xQueueRemoveFromSet( QueueSetMemberHandle_t xQueueOrSemaphore,
                                        QueueSetHandle_t xQueueSet ) /* FREERTOS_SYSTEM_CALL */
    {
        BaseType_t xReturn;
        xPortRaisePrivilege();

        xReturn = xQueueRemoveFromSet( xQueueOrSemaphore, xQueueSet );
        vPortResetPrivilege();
        return xReturn;
    }
#endif /* if ( configUSE_QUEUE_SETS == 1 ) */
/*-----------------------------------------------------------*/

#if configQUEUE_REGISTRY_SIZE > 0
    void MPU_vQueueAddToRegistry( QueueHandle_t xQueue,
                                  const char * pcName ) /* FREERTOS_SYSTEM_CALL */
    {
        xPortRaisePrivilege();

        vQueueAddToRegistry( xQueue, pcName );

        vPortResetPrivilege();
    }
#endif
/*-----------------------------------------------------------*/

#if configQUEUE_REGISTRY_SIZE > 0
    void MPU_vQueueUnregisterQueue( QueueHandle_t xQueue ) /* FREERTOS_SYSTEM_CALL */
    {
        xPortRaisePrivilege();

        vQueueUnregisterQueue( xQueue );

        vPortResetPrivilege();
    }
#endif
/*-----------------------------------------------------------*/

#if configQUEUE_REGISTRY_SIZE > 0
    const char * MPU_pcQueueGetName( QueueHandle_t xQueue ) /* FREERTOS_SYSTEM_CALL */
    {
        xPortRaisePrivilege();
        const char * pcReturn;

        pcReturn = pcQueueGetName( xQueue );

        vPortResetPrivilege();
        return pcReturn;
    }
#endif /* if configQUEUE_REGISTRY_SIZE > 0 */
/*-----------------------------------------------------------*/

void MPU_vQueueDelete( QueueHandle_t xQueue ) /* FREERTOS_SYSTEM_CALL */
{
    xPortRaisePrivilege();

    vQueueDelete( xQueue );

    vPortResetPrivilege();
}
/*-----------------------------------------------------------*/

#if ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
    void MPU_vPortInitialiseBlocks( void ) /* FREERTOS_SYSTEM_CALL */
    {
        xPortRaisePrivilege();

        vPortInitialiseBlocks();

        vPortResetPrivilege();
    }
#endif /* configSUPPORT_DYNAMIC_ALLOCATION */
/*-----------------------------------------------------------*/

#if ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
    size_t MPU_xPortGetFreeHeapSize( void ) /* FREERTOS_SYSTEM_CALL */
    {
        size_t xReturn;
        xPortRaisePrivilege();

        xReturn = xPortGetFreeHeapSize();

        vPortResetPrivilege();

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
        xPortRaisePrivilege();

        xReturn = xTimerCreate( pcTimerName, xTimerPeriodInTicks, uxAutoReload, pvTimerID, pxCallbackFunction );
        vPortResetPrivilege();

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
        xPortRaisePrivilege();

        xReturn = xTimerCreateStatic( pcTimerName, xTimerPeriodInTicks, uxAutoReload, pvTimerID, pxCallbackFunction, pxTimerBuffer );
        vPortResetPrivilege();

        return xReturn;
    }
#endif /* if ( ( configSUPPORT_STATIC_ALLOCATION == 1 ) && ( configUSE_TIMERS == 1 ) ) */
/*-----------------------------------------------------------*/

#if ( configUSE_TIMERS == 1 )
    void * MPU_pvTimerGetTimerID( const TimerHandle_t xTimer ) /* FREERTOS_SYSTEM_CALL */
    {
        void * pvReturn;
        xPortRaisePrivilege();

        pvReturn = pvTimerGetTimerID( xTimer );
        vPortResetPrivilege();

        return pvReturn;
    }
#endif /* if ( configUSE_TIMERS == 1 ) */
/*-----------------------------------------------------------*/

#if ( configUSE_TIMERS == 1 )
    void MPU_vTimerSetTimerID( TimerHandle_t xTimer,
                               void * pvNewID ) /* FREERTOS_SYSTEM_CALL */
    {
        xPortRaisePrivilege();

        vTimerSetTimerID( xTimer, pvNewID );
        vPortResetPrivilege();
    }
#endif
/*-----------------------------------------------------------*/

#if ( configUSE_TIMERS == 1 )
    BaseType_t MPU_xTimerIsTimerActive( TimerHandle_t xTimer ) /* FREERTOS_SYSTEM_CALL */
    {
        BaseType_t xReturn;
        xPortRaisePrivilege();

        xReturn = xTimerIsTimerActive( xTimer );
        vPortResetPrivilege();

        return xReturn;
    }
#endif /* if ( configUSE_TIMERS == 1 ) */
/*-----------------------------------------------------------*/

#if ( configUSE_TIMERS == 1 )
    TaskHandle_t MPU_xTimerGetTimerDaemonTaskHandle( void ) /* FREERTOS_SYSTEM_CALL */
    {
        TaskHandle_t xReturn;
        xPortRaisePrivilege();

        xReturn = xTimerGetTimerDaemonTaskHandle();
        vPortResetPrivilege();

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
        xPortRaisePrivilege();

        xReturn = xTimerPendFunctionCall( xFunctionToPend, pvParameter1, ulParameter2, xTicksToWait );
        vPortResetPrivilege();

        return xReturn;
    }
#endif /* if ( ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 1 ) ) */
/*-----------------------------------------------------------*/

#if ( configUSE_TIMERS == 1 )
    void MPU_vTimerSetReloadMode( TimerHandle_t xTimer,
                                  const UBaseType_t uxAutoReload ) /* FREERTOS_SYSTEM_CALL */
    {
        xPortRaisePrivilege();

        vTimerSetReloadMode( xTimer, uxAutoReload );
        vPortResetPrivilege();
    }
#endif
/*-----------------------------------------------------------*/

#if ( configUSE_TIMERS == 1 )
    UBaseType_t MPU_uxTimerGetReloadMode( TimerHandle_t xTimer )
    {
        xPortRaisePrivilege();
        UBaseType_t uxReturn;

        uxReturn = uxTimerGetReloadMode( xTimer );
        vPortResetPrivilege();
        return uxReturn;
    }
#endif
/*-----------------------------------------------------------*/

#if ( configUSE_TIMERS == 1 )
    const char * MPU_pcTimerGetName( TimerHandle_t xTimer ) /* FREERTOS_SYSTEM_CALL */
    {
        const char * pcReturn;
        xPortRaisePrivilege();

        pcReturn = pcTimerGetName( xTimer );
        vPortResetPrivilege();

        return pcReturn;
    }
#endif /* if ( configUSE_TIMERS == 1 ) */
/*-----------------------------------------------------------*/

#if ( configUSE_TIMERS == 1 )
    TickType_t MPU_xTimerGetPeriod( TimerHandle_t xTimer ) /* FREERTOS_SYSTEM_CALL */
    {
        TickType_t xReturn;
        xPortRaisePrivilege();

        xReturn = xTimerGetPeriod( xTimer );
        vPortResetPrivilege();

        return xReturn;
    }
#endif /* if ( configUSE_TIMERS == 1 ) */
/*-----------------------------------------------------------*/

#if ( configUSE_TIMERS == 1 )
    TickType_t MPU_xTimerGetExpiryTime( TimerHandle_t xTimer ) /* FREERTOS_SYSTEM_CALL */
    {
        TickType_t xReturn;
        xPortRaisePrivilege();

        xReturn = xTimerGetExpiryTime( xTimer );
        vPortResetPrivilege();

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
        xPortRaisePrivilege();

        xReturn = xTimerGenericCommand( xTimer, xCommandID, xOptionalValue, pxHigherPriorityTaskWoken, xTicksToWait );
        vPortResetPrivilege();

        return xReturn;
    }
#endif /* if ( configUSE_TIMERS == 1 ) */
/*-----------------------------------------------------------*/

#if ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
    EventGroupHandle_t MPU_xEventGroupCreate( void ) /* FREERTOS_SYSTEM_CALL */
    {
        EventGroupHandle_t xReturn;
        xPortRaisePrivilege();

        xReturn = xEventGroupCreate();
        vPortResetPrivilege();

        return xReturn;
    }
#endif /* if ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) */
/*-----------------------------------------------------------*/

#if ( configSUPPORT_STATIC_ALLOCATION == 1 )
    EventGroupHandle_t MPU_xEventGroupCreateStatic( StaticEventGroup_t * pxEventGroupBuffer ) /* FREERTOS_SYSTEM_CALL */
    {
        EventGroupHandle_t xReturn;
        xPortRaisePrivilege();

        xReturn = xEventGroupCreateStatic( pxEventGroupBuffer );
        vPortResetPrivilege();

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
    xPortRaisePrivilege();

    xReturn = xEventGroupWaitBits( xEventGroup, uxBitsToWaitFor, xClearOnExit, xWaitForAllBits, xTicksToWait );
    vPortResetPrivilege();

    return xReturn;
}
/*-----------------------------------------------------------*/

EventBits_t MPU_xEventGroupClearBits( EventGroupHandle_t xEventGroup,
                                      const EventBits_t uxBitsToClear ) /* FREERTOS_SYSTEM_CALL */
{
    EventBits_t xReturn;
    xPortRaisePrivilege();

    xReturn = xEventGroupClearBits( xEventGroup, uxBitsToClear );
    vPortResetPrivilege();

    return xReturn;
}
/*-----------------------------------------------------------*/

EventBits_t MPU_xEventGroupSetBits( EventGroupHandle_t xEventGroup,
                                    const EventBits_t uxBitsToSet ) /* FREERTOS_SYSTEM_CALL */
{
    EventBits_t xReturn;
    xPortRaisePrivilege();

    xReturn = xEventGroupSetBits( xEventGroup, uxBitsToSet );
    vPortResetPrivilege();

    return xReturn;
}
/*-----------------------------------------------------------*/

EventBits_t MPU_xEventGroupSync( EventGroupHandle_t xEventGroup,
                                 const EventBits_t uxBitsToSet,
                                 const EventBits_t uxBitsToWaitFor,
                                 TickType_t xTicksToWait ) /* FREERTOS_SYSTEM_CALL */
{
    EventBits_t xReturn;
    xPortRaisePrivilege();

    xReturn = xEventGroupSync( xEventGroup, uxBitsToSet, uxBitsToWaitFor, xTicksToWait );
    vPortResetPrivilege();

    return xReturn;
}
/*-----------------------------------------------------------*/

void MPU_vEventGroupDelete( EventGroupHandle_t xEventGroup ) /* FREERTOS_SYSTEM_CALL */
{
    xPortRaisePrivilege();

    vEventGroupDelete( xEventGroup );
    vPortResetPrivilege();
}
/*-----------------------------------------------------------*/

size_t MPU_xStreamBufferSend( StreamBufferHandle_t xStreamBuffer,
                              const void * pvTxData,
                              size_t xDataLengthBytes,
                              TickType_t xTicksToWait ) /* FREERTOS_SYSTEM_CALL */
{
    size_t xReturn;
    xPortRaisePrivilege();

    xReturn = xStreamBufferSend( xStreamBuffer, pvTxData, xDataLengthBytes, xTicksToWait );
    vPortResetPrivilege();

    return xReturn;
}
/*-----------------------------------------------------------*/

size_t MPU_xStreamBufferNextMessageLengthBytes( StreamBufferHandle_t xStreamBuffer ) /* FREERTOS_SYSTEM_CALL */
{
    size_t xReturn;
    xPortRaisePrivilege();

    xReturn = xStreamBufferNextMessageLengthBytes( xStreamBuffer );
    vPortResetPrivilege();

    return xReturn;
}
/*-----------------------------------------------------------*/

size_t MPU_xStreamBufferReceive( StreamBufferHandle_t xStreamBuffer,
                                 void * pvRxData,
                                 size_t xBufferLengthBytes,
                                 TickType_t xTicksToWait ) /* FREERTOS_SYSTEM_CALL */
{
    size_t xReturn;
    xPortRaisePrivilege();

    xReturn = xStreamBufferReceive( xStreamBuffer, pvRxData, xBufferLengthBytes, xTicksToWait );
    vPortResetPrivilege();

    return xReturn;
}
/*-----------------------------------------------------------*/

void MPU_vStreamBufferDelete( StreamBufferHandle_t xStreamBuffer ) /* FREERTOS_SYSTEM_CALL */
{
    xPortRaisePrivilege();

    vStreamBufferDelete( xStreamBuffer );
    vPortResetPrivilege();
}
/*-----------------------------------------------------------*/

BaseType_t MPU_xStreamBufferIsFull( StreamBufferHandle_t xStreamBuffer ) /* FREERTOS_SYSTEM_CALL */
{
    BaseType_t xReturn;
    xPortRaisePrivilege();

    xReturn = xStreamBufferIsFull( xStreamBuffer );
    vPortResetPrivilege();

    return xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t MPU_xStreamBufferIsEmpty( StreamBufferHandle_t xStreamBuffer ) /* FREERTOS_SYSTEM_CALL */
{
    BaseType_t xReturn;
    xPortRaisePrivilege();

    xReturn = xStreamBufferIsEmpty( xStreamBuffer );
    vPortResetPrivilege();

    return xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t MPU_xStreamBufferReset( StreamBufferHandle_t xStreamBuffer ) /* FREERTOS_SYSTEM_CALL */
{
    BaseType_t xReturn;
    xPortRaisePrivilege();

    xReturn = xStreamBufferReset( xStreamBuffer );
    vPortResetPrivilege();

    return xReturn;
}
/*-----------------------------------------------------------*/

size_t MPU_xStreamBufferSpacesAvailable( StreamBufferHandle_t xStreamBuffer ) /* FREERTOS_SYSTEM_CALL */
{
    size_t xReturn;
    xPortRaisePrivilege();

    xReturn = xStreamBufferSpacesAvailable( xStreamBuffer );
    vPortResetPrivilege();

    return xReturn;
}
/*-----------------------------------------------------------*/

size_t MPU_xStreamBufferBytesAvailable( StreamBufferHandle_t xStreamBuffer ) /* FREERTOS_SYSTEM_CALL */
{
    size_t xReturn;
    xPortRaisePrivilege();

    xReturn = xStreamBufferBytesAvailable( xStreamBuffer );
    vPortResetPrivilege();

    return xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t MPU_xStreamBufferSetTriggerLevel( StreamBufferHandle_t xStreamBuffer,
                                             size_t xTriggerLevel ) /* FREERTOS_SYSTEM_CALL */
{
    BaseType_t xReturn;
    xPortRaisePrivilege();

    xReturn = xStreamBufferSetTriggerLevel( xStreamBuffer, xTriggerLevel );
    vPortResetPrivilege();

    return xReturn;
}
/*-----------------------------------------------------------*/

#if ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
    StreamBufferHandle_t MPU_xStreamBufferGenericCreate( size_t xBufferSizeBytes,
                                                         size_t xTriggerLevelBytes,
                                                         BaseType_t xIsMessageBuffer ) /* FREERTOS_SYSTEM_CALL */
    {
        StreamBufferHandle_t xReturn;
        xPortRaisePrivilege();

        xReturn = xStreamBufferGenericCreate( xBufferSizeBytes, xTriggerLevelBytes, xIsMessageBuffer );
        vPortResetPrivilege();

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
        xPortRaisePrivilege();

        xReturn = xStreamBufferGenericCreateStatic( xBufferSizeBytes, xTriggerLevelBytes, xIsMessageBuffer, pucStreamBufferStorageArea, pxStaticStreamBuffer );
        vPortResetPrivilege();

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
 * xPortRaisePrivilege();
 *
 *  FunctionName( [parameters ] );
 *
 *  vPortResetPrivilege();
 * }
 */

#if configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS == 1
    #include "application_defined_privileged_functions.h"
#endif
