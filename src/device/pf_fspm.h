/*********************************************************************
 *        _       _         _
 *  _ __ | |_  _ | |  __ _ | |__   ___
 * | '__|| __|(_)| | / _` || '_ \ / __|
 * | |   | |_  _ | || (_| || |_) |\__ \
 * |_|    \__|(_)|_| \__,_||_.__/ |___/
 *
 * www.rt-labs.com
 * Copyright 2018 rt-labs AB, Sweden.
 *
 * This software is dual-licensed under GPLv3 and a commercial
 * license. See the file LICENSE.md distributed with this software for
 * full license information.
 ********************************************************************/

/**
 * @file
 * @brief Internal header for FSPM
 */

#ifndef PF_FSPM_H
#define PF_FSPM_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

#include "pf_includes.h"

/**
 * Initialize the FSPM component.
 * @param p_cfg            In:   The application configuration of the Profinet stack.
 * @return  0  if operation succeeded.
 *          -1 if an error occurred.
 */
int pf_fspm_init(
   const pnet_cfg_t        *p_cfg);

/**
 * Create a LogBook entry.
 * @param arep             In:   The AREP, indentifying the AR.
 * @param p_pnio_status    In:   The PNIO status.
 * @param entry_detail     In:   Additional application information.
 */
void pf_fspm_create_log_book_entry(
   uint32_t                arep,
   const pnet_pnio_status_t *p_pnio_status,
   uint32_t                entry_detail);

/**
 * Process write record requests from the controller.
 * If index is user-defined then call application call-back (if defined).
 * If index indicates I&M data records then handle here.
 * @param p_ar             In:   The AR instance.
 * @param p_write_request  In:   The write request record.
 * @param write_length     In:   Length in bytes of write data.
 * @param p_write_data     In:   The data to write.
 * @param p_result         Out:  Result informantion.
 * @return  0  if operation succeeded.
 *          -1 if an error occurred.
 */
int pf_fspm_cm_write_ind(
   pf_ar_t                 *p_ar,
   pf_iod_write_request_t  *p_write_request,
   uint16_t                write_length,
   uint8_t                 *p_write_data,
   pnet_result_t           *p_result);

/**
 * Process read record requests from the controller.
 * @param p_ar             In:   The AR instance.
 * @param p_read_request   In:   The read request record.
 * @param pp_read_data     Out:  A pointer to the source data.
 * @param p_read_length    Out:  Size of the source data.
 * @param p_result         Out:  The result information.
 * @return  0  if operation succeeded.
 *          -1 if an error occurred.
 */
int pf_fspm_cm_read_ind(
   pf_ar_t                 *p_ar,
   pf_iod_read_request_t   *p_read_request,
   uint8_t                 **pp_read_data,
   uint16_t                *p_read_length,
   pnet_result_t           *p_result);

/**
 * Response from controller to appl_rdy request.
 * @param p_ar             In:   The AR instance.
 * @param p_result         Out:  The result information.
 * @return  0  if operation succeeded.
 *          -1 if an error occurred.
 */
int pf_fspm_ccontrol_cnf(
   pf_ar_t                 *p_ar,
   pnet_result_t           *p_result);

/**
 * Call user call-back when a new connection is requested.
 * @param p_ar             In:   The AR instance.
 * @param p_result         Out:  The result information.
 * @return  0  if operation succeeded.
 *          -1 if an error occurred.
 */
int pf_fspm_cm_connect_ind(
   pf_ar_t                 *p_ar,
   pnet_result_t           *p_result);

/**
 * Call user call-back when a connection is released.
 * @param p_ar             In:   The AR instance.
 * @param p_result         Out:  The result information.
 * @return  0  if operation succeeded.
 *          -1 if an error occurred.
 */
int pf_fspm_cm_release_ind(
   pf_ar_t                 *p_ar,
   pnet_result_t           *p_result);

/**
 * Call user call-back to indicate a control from the ProfiNet controller.
 * @param p_ar             In:   The AR instance.
 * @param control_command  In:   The control command.
 * @param p_result         Out:  The result information.
 * @return  0  if operation succeeded.
 *          -1 if an error occurred.
 */
int pf_fspm_cm_dcontrol_ind(
   pf_ar_t                 *p_ar,
   pnet_control_command_t  control_command,
   pnet_result_t           *p_result);

/**
 * Call user call-back to indicate a new event within the profinet stack.
 * @param p_ar             In:   The AR instance.
 * @param event            In:   The CMDEV event.
 * @return  0  if operation succeeded.
 *          -1 if an error occurred.
 */
int pf_fspm_state_ind(
   pf_ar_t                 *p_ar,
   pnet_event_values_t     event);

/**
 * The remote side sends an alarm.
 * @param p_ar             In:   The AR instance.
 * @return  0  if operation succeeded.
 *          -1 if an error occurred.
 */
int pf_fspm_aplmr_alarm_ind(
   pf_ar_t                 *p_ar,
   uint32_t                api,
   uint16_t                slot,
   uint16_t                subslot,
   uint16_t                data_len,
   uint16_t                data_usi,
   uint8_t                 *p_data);

/**
 * The remote side acknowledges the alarm sent by this side by sending an AlarmAck.
 * @param p_ar             In:   The AR instance.
 * @param p_pnio_status    In:   Detailed ACK information.
 * @return  0  if operation succeeded.
 *          -1 if an error occurred.
 */
int pf_fspm_aplmi_alarm_cnf(
   pf_ar_t                 *p_ar,
   pnet_pnio_status_t      *p_pnio_status);

/**
 * Local side has successfully sent the AlarmAck to the remote side.
 * @param p_ar             In:   The AR instance.
 * @param res              In:   0  ACk was received by remote side.
 *                               -1 ACk was not received by remote side.
 * @return  0  if operation succeeded.
 *          -1 if an error occurred.
 */
int pf_fspm_aplmr_alarm_ack_cnf(
   pf_ar_t                 *p_ar,
   int                     res);

/**
 * Call user call-back to indicate to application it needs a specific module in a specific slot.
 * @param api_id           In:   The API id.
 * @param slot_nbr         In:   The slot number.
 * @param module_ident     In:   The module ident number.
 * @return  0  if operation succeeded.
 *          -1 if an error occurred.
 */
int pf_fspm_exp_module_ind(
   uint32_t                api_id,
   uint16_t                slot_nbr,
   uint32_t                module_ident);

/**
 * Call user call-back to indicate to application it needs a specific sub-module in a specific sub-slot.
 * @param api_id           In:   The API id.
 * @param slot_nbr         In:   The slot number.
 * @param subslot_nbr      In:   The sub-slot number.
 * @param module_ident     In:   The module ident number.
 * @param submodule_ident  In:   The sub-module ident number.
 * @return  0  if operation succeeded.
 *          -1 if an error occurred.
 */
int pf_fspm_exp_submodule_ind(
   uint32_t                api_id,
   uint16_t                slot_nbr,
   uint16_t                subslot_nbr,
   uint32_t                module_ident,
   uint32_t                submodule_ident);

/**
 * Notify application that the received data status has changed.
 * @param p_ar             In:  The AR instance.
 * @param p_iocr           In:  The IOCR instance.
 * @param changes          In:  The changed bits in the data status.
 * @return  0  if operation succeeded.
 *          -1 if an error occurred.
 */
int pf_fspm_data_status_changed(
   pf_ar_t                 *p_ar,
   pf_iocr_t               *p_iocr,
   uint8_t                 changes);

/**
 * Retrieve a pointer to the current configuration data.
 * @param pp_cfg           Out: The current configuration (or NULL).
 */
void pf_fspm_get_cfg(
   pnet_cfg_t              **pp_cfg);

/**
 * Retrieve a pointer to the default configuration data.
 * @param pp_cfg           Out: The default configuration (or NULL).
 */
void pf_fspm_get_default_cfg(
   const pnet_cfg_t        **pp_cfg);

/**
 * Clear the I&M data records 1-4.
 * @return  0  if operation succeeded.
 *          -1 if an error occurred.
 */
int pf_fspm_clear_im_data(void);

#ifdef __cplusplus
}
#endif

#endif /* PF_FSPM_H */