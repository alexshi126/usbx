/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation
 *
 * This program and the accompanying materials are made available under the
 * terms of the MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 **************************************************************************/

/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** USBX Component                                                        */
/**                                                                       */
/**   Device Audio Class                                                    */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/

#define UX_SOURCE_CODE


/* Include necessary system files.  */

#include "ux_api.h"
#include "ux_device_class_audio.h"
#include "ux_device_stack.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _ux_device_class_audio_entry                        PORTABLE C      */
/*                                                           6.x          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Chaoqiong Xiao, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function is the entry point of the audio class. It             */
/*    will be called by the device stack enumeration module when the      */
/*    host has sent a SET_CONFIGURATION command and the audio interface   */
/*    needs to be mounted.                                                */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    command                               Pointer to class command      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion Status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _ux_device_class_audio_initialize         Initialize audio class    */
/*    _ux_device_class_audio_uninitialize       Uninitialize audio class  */
/*    _ux_device_class_audio_activate           Activate audio class      */
/*    _ux_device_class_audio_change             Change audio interface    */
/*    _ux_device_class_audio_deactivate         Deactivate audio class    */
/*    _ux_device_class_audio_control_request    Request control           */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Device Stack                                                        */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Chaoqiong Xiao           Initial Version 6.0           */
/*  09-30-2020     Chaoqiong Xiao           Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  07-29-2022     Chaoqiong Xiao           Modified comment(s),          */
/*                                            returned request status,    */
/*                                            resulting in version 6.1.12 */
/*  03-08-2023     Chaoqiong Xiao           Modified comment(s),          */
/*                                            added error checks support, */
/*                                            resulting in version 6.2.1  */
/*  xx-xx-xxxx     Mohamed ayed             Modified comment(s),          */
/*                                            fix typo,                   */
/*                                            resulting in version 6.x    */
/*                                                                        */
/**************************************************************************/
UINT  _ux_device_class_audio_entry(UX_SLAVE_CLASS_COMMAND *command)
{

UINT        status;


    /* The command request will tell us we need to do here, either a enumeration
       query, an activation or a deactivation.  */
    switch (command -> ux_slave_class_command_request)
    {

    case UX_SLAVE_CLASS_COMMAND_INITIALIZE:

        /* Call the init function of the Audio class.  */
#if defined(UX_DEVICE_CLASS_AUDIO_ENABLE_ERROR_CHECKING)
        status =  _uxe_device_class_audio_initialize(command);
#else
        status =  _ux_device_class_audio_initialize(command);
#endif

        /* Return the completion status.  */
        return(status);

    case UX_SLAVE_CLASS_COMMAND_UNINITIALIZE:

        /* Call the uninit function of the Audio class.  */
        status =  _ux_device_class_audio_uninitialize(command);

        /* Return the completion status.  */
        return(status);

    case UX_SLAVE_CLASS_COMMAND_QUERY:

        /* Check the CLASS definition in the interface descriptor. */
        if (command -> ux_slave_class_command_class == UX_DEVICE_CLASS_AUDIO_CLASS)
        {
            if (command -> ux_slave_class_command_subclass == UX_DEVICE_CLASS_AUDIO_SUBCLASS_CONTROL)
                return(UX_SUCCESS);
            if (command -> ux_slave_class_command_subclass == UX_DEVICE_CLASS_AUDIO_SUBCLASS_AUDIOSTREAMING)
                return(UX_SUCCESS);
        }
        return(UX_NO_CLASS_MATCH);

    case UX_SLAVE_CLASS_COMMAND_ACTIVATE:

        /* The activate command is used when the host has sent a SET_CONFIGURATION command
           and this interface has to be mounted. Both Bulk endpoints have to be mounted
           and the audio thread needs to be activated.  */
        status =  _ux_device_class_audio_activate(command);

        /* Return the completion status.  */
        return(status);

    case UX_SLAVE_CLASS_COMMAND_CHANGE:

        /* The change command is used when the host has sent a SET_INTERFACE command
           to go from Alternate Setting 0 to 1 or revert to the default mode.  */
        status =  _ux_device_class_audio_change(command);

        /* Return the completion status.  */
        return(status);

    case UX_SLAVE_CLASS_COMMAND_DEACTIVATE:

        /* The deactivate command is used when the device has been extracted.
           The device endpoints have to be dismounted and the audio thread canceled.  */
        status =  _ux_device_class_audio_deactivate(command);

        /* Return the completion status.  */
        return(status);

    case UX_SLAVE_CLASS_COMMAND_REQUEST:

        /* The request command is used when the host sends a command on the control endpoint.  */
        return _ux_device_class_audio_control_request(command);

    default:

        /* Return an error.  */
        return(UX_FUNCTION_NOT_SUPPORTED);
    }
}
