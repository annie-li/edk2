/** @file
  Header file for CxlDxe driver
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _EFI_CXLDXE_H_
#define _EFI_CXLDXE_H_

#include <Protocol/PciIo.h>
#include <IndustryStandard/Pci.h>
#include <IndustryStandard/Cxl20.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Base.h>

#if defined(_MSC_VER)
#include <intrin.h>  /* __popcnt64 */
#endif

#define CXL_MEMORY_CLASS      0x05
#define CXL_MEMORY_SUB_CLASS  0x02
#define CXL_MEMORY_PROGIF     0x10
#define CXL_IS_DVSEC(n)  (((n) & (0xFFFF)) == CXL_PCI_DVSEC_VENDOR_ID)
#define CXL_CONTROLLER_PRIVATE_DATA_SIGNATURE  SIGNATURE_32 ('C','X','L','X')
#define CXL_PCI_EXT_CAP_ID(Header)    (Header & 0x0000ffff)
#define CXL_PCI_EXT_CAP_NEXT(Header)  ((Header >> 20) & 0xfff)
#define CXL_DEV_CAP_ARRAY_OFFSET  0x0
#define CXL_DEV_CAP_ARRAY_CAP_ID  0
#define CXL_BIT(nr)  ((UINT32)1 << nr)
#define CXL_DEV_MBOX_CTRL_DOORBELL  CXL_BIT(0)
#define CXL_SZ_1M                   0x00100000
#define CXL_BITS_PER_LONG           32
#define CXL_UL                      (UINTN)

#define CXL_GENMASK(h, l) \
  (((~CXL_UL(0)) - (CXL_UL(1) << (l)) + 1) & \
    (~CXL_UL(0) >> (CXL_BITS_PER_LONG - 1 - (h))))

//
// CXL Memory Device Register information
//
typedef struct {
  UINT32    RegisterType;
  UINT32    BaseAddressRegister;
  UINT64    RegisterOffset;
  UINT32    MailboxRegistersOffset;
} CXL_REGISTER_MAP;

//
// CXL Memory Device Registers state
//
typedef struct {
  UINT32    PayloadSize;
} CXL_MEMDEV_STATE;

//
// CXL device private data structure
//
typedef struct {
  UINT32                      Signature;
  EFI_HANDLE                  ControllerHandle;
  EFI_HANDLE                  ImageHandle;
  EFI_HANDLE                  DriverBindingHandle;
  EFI_PCI_IO_PROTOCOL         *PciIo;
  EFI_DEVICE_PATH_PROTOCOL    *ParentDevicePath;

  // MailBox Register
  CXL_REGISTER_MAP            RegisterMap;
  CXL_MEMDEV_STATE            MemdevState;
  CXL_MBOX_CMD                MailboxCmd;

  // BDF Value
  UINTN                       Seg;
  UINTN                       Bus;
  UINTN                       Device;
  UINTN                       Function;
} CXL_CONTROLLER_PRIVATE_DATA;

/**
  Tests to see if this driver supports a given controller. If a child device is provided,
  it further tests to see if this driver supports creating a handle for the specified child device.

  This function checks to see if the driver specified by This supports the device specified by
  ControllerHandle. Drivers will typically use the device path attached to
  ControllerHandle and/or the services from the bus I/O abstraction attached to
  ControllerHandle to determine if the driver supports ControllerHandle. This function
  may be called many times during platform initialization. In order to reduce boot times, the tests
  performed by this function must be very small, and take as little time as possible to execute. This
  function must not change the State of any hardware devices, and this function must be aware that the
  device specified by ControllerHandle may already be managed by the same driver or a
  different driver. This function must match its calls to AllocatePages() with FreePages(),
  AllocatePool() with FreePool(), and OpenProtocol() with CloseProtocol().
  Since ControllerHandle may have been previously started by the same driver, if a protocol is
  already in the opened State, then it must not be closed with CloseProtocol(). This is required
  to guarantee the State of ControllerHandle is not modified by this function.

  @param[in]  This                 A pointer to the EFI_DRIVER_BINDING_PROTOCOL instance.

  @param[in]  ControllerHandle     The handle of the controller to test. This handle
                                   must support a protocol interface that supplies
                                   an I/O abstraction to the driver.

  @param[in]  RemainingDevicePath  A pointer to the remaining portion of a device path.  This
                                   parameter is ignored by device drivers, and is optional for bus
                                   drivers. For bus drivers, if this parameter is not NULL, then
                                   the bus driver must determine if the bus controller specified
                                   by ControllerHandle and the child controller specified
                                   by RemainingDevicePath are both supported by this
                                   bus driver.

  @retval EFI_SUCCESS              The device specified by ControllerHandle and
                                   RemainingDevicePath is supported by the driver specified by This.

  @retval EFI_ALREADY_STARTED      The device specified by ControllerHandle and
                                   RemainingDevicePath is already being managed by the driver
                                   specified by This.

  @retval EFI_ACCESS_DENIED        The device specified by ControllerHandle and
                                   RemainingDevicePath is already being managed by a different
                                   driver or an application that requires exclusive access.
                                   Currently not implemented.

  @retval EFI_UNSUPPORTED          The device specified by ControllerHandle and
                                   RemainingDevicePath is not supported by the driver specified by This.
**/
EFI_STATUS
EFIAPI
CxlDriverBindingSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   Controller,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath
  );

/**
  Starts a device controller or a bus controller.

  The Start() function is designed to be invoked from the EFI boot service ConnectController().
  As a result, much of the error checking on the parameters to Start() has been moved into this
  common boot service. It is legal to call Start() from other locations,
  but the following calling restrictions must be followed or the system behavior will not be deterministic.
  1. ControllerHandle must be a valid EFI_HANDLE.
  2. If RemainingDevicePath is not NULL, then it must be a pointer to a naturally aligned
     EFI_DEVICE_PATH_PROTOCOL.
  3. Prior to calling Start(), the Supported() function for the driver specified by This must
     have been called with the same calling parameters, and Supported() must have returned EFI_SUCCESS.

  @param[in]  This                 A pointer to the EFI_DRIVER_BINDING_PROTOCOL instance.

  @param[in]  ControllerHandle     The handle of the controller to Start. This handle
                                   must support a protocol interface that supplies
                                   an I/O abstraction to the driver.
  @param[in]  RemainingDevicePath  A pointer to the remaining portion of a device path.  This
                                   parameter is ignored by device drivers, and is optional for bus
                                   drivers. For a bus driver, if this parameter is NULL, then handles
                                   for all the children of Controller are created by this driver.
                                   If this parameter is not NULL and the first Device Path Node is
                                   not the End of Device Path Node, then only the handle for the
                                   child device specified by the first Device Path Node of
                                   RemainingDevicePath is created by this driver.
                                   If the first Device Path Node of RemainingDevicePath is
                                   the End of Device Path Node, no child handle is created by this
                                   driver.

  @retval EFI_SUCCESS              The device was started.

  @retval EFI_DEVICE_ERROR         The device could not be started due to a device error.Currently not implemented.

  @retval EFI_OUT_OF_RESOURCES     The request could not be completed due to a lack of resources.

  @retval Others                   The driver failded to Start the device.

**/
EFI_STATUS
EFIAPI
CxlDriverBindingStart (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   Controller,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath
  );

/**
  Stops a device controller or a bus controller.

  The Stop() function is designed to be invoked from the EFI boot service DisconnectController().
  As a result, much of the error checking on the parameters to Stop() has been moved
  into this common boot service. It is legal to call Stop() from other locations,
  but the following calling restrictions must be followed or the system behavior will not be deterministic.
  1. ControllerHandle must be a valid EFI_HANDLE that was used on a previous call to this
     same driver's Start() function.
  2. The first NumberOfChildren handles of ChildHandleBuffer must all be a valid
     EFI_HANDLE. In addition, all of these handles must have been created in this driver's
     Start() function, and the Start() function must have called OpenProtocol() on
     ControllerHandle with an Attribute of EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER.

  @param[in]  This               A pointer to the EFI_DRIVER_BINDING_PROTOCOL instance.

  @param[in]  ControllerHandle   A handle to the device being stopped. The handle must
                                 support a bus specific I/O protocol for the driver
                                 to use to stop the device.
  @param[in]  NumberOfChildren   The number of child device handles in ChildHandleBuffer.

  @param[in]  ChildHandleBuffer  An array of child handles to be freed. May be NULL
                                 if NumberOfChildren is 0.

  @retval EFI_SUCCESS            The device was stopped.

  @retval EFI_DEVICE_ERROR       The device could not be stopped due to a device error.

**/
EFI_STATUS
EFIAPI
CxlDriverBindingStop (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   Controller,
  IN  UINTN                       NumberOfChildren,
  IN  EFI_HANDLE                  *ChildHandleBuffer
  );

/**
  Issue a command to the device using mailbox registers

  @param[in] Private                  The pointer to the CXL_CONTROLLER_PRIVATE_DATA data structure.

  @retval Status                      Return EFI_SUCCESS on successfully calling CxlDecodeRegblock

**/
EFI_STATUS
CxlPciMboxSend (
  CXL_CONTROLLER_PRIVATE_DATA  *Private
  );

#endif // _EFI_CXLDXE_H_

