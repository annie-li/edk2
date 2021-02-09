/** @file
*  OemMiscLib.c
*
*  Copyright (c) 2021, NUVIA Inc. All rights reserved.
*  Copyright (c) 2018, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2018, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <Uefi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HiiLib.h>

#include <Library/OemMiscLib.h>


/** Gets the CPU frequency of the specified processor.

  @param ProcessorIndex Index of the processor to get the frequency for.

  @return               CPU frequency in Hz
**/
EFIAPI
UINTN
OemGetCpuFreq (
  IN UINT8 ProcessorIndex
  )
{
  ASSERT (FALSE);
  return 0;
}

/** Gets information about the specified processor and stores it in
    the structures provided.

  @param ProcessorIndex  Index of the processor to get the information for.
  @param ProcessorStatus Processor status.
  @param ProcessorCharacteristics Processor characteritics.
  @param MiscProcessorData        Miscellaneous processor information.

  @return  TRUE on success, FALSE on failure.
**/
EFIAPI
BOOLEAN
OemGetProcessorInformation (
  IN UINTN ProcessorIndex,
  IN OUT PROCESSOR_STATUS_DATA *ProcessorStatus,
  IN OUT PROCESSOR_CHARACTERISTIC_FLAGS *ProcessorCharacteristics,
  IN OUT OEM_MISC_PROCESSOR_DATA *MiscProcessorData
  )
{
  ASSERT (FALSE);
  return TRUE;
}

/** Gets information about the cache at the specified cache level.

  @param ProcessorIndex The processor to get information for.
  @param CacheLevel The cache level to get information for.
  @param DataCache  Whether the cache is a data cache.
  @param UnifiedCache Whether the cache is a unified cache.
  @param SmbiosCacheTable The SMBIOS Type7 cache information structure.

  @return TRUE on success, FALSE on failure.
**/
EFIAPI
BOOLEAN
OemGetCacheInformation (
  IN UINT8   ProcessorIndex,
  IN UINT8   CacheLevel,
  IN BOOLEAN DataCache,
  IN BOOLEAN UnifiedCache,
  IN OUT SMBIOS_TABLE_TYPE7 *SmbiosCacheTable
  )
{
  ASSERT (FALSE);
  return TRUE;
}

/** Gets the maximum number of sockets supported by the platform.

  @return The maximum number of sockets.
**/
EFIAPI
UINT8
OemGetProcessorMaxSockets (
  VOID
  )
{
  ASSERT (FALSE);
  return 1;
}

/** Gets the type of chassis for the system.

  @param ChassisType The type of the chassis.

  @retval EFI_SUCCESS The chassis type was fetched successfully.
**/
EFI_STATUS
EFIAPI
OemGetChassisType (
  UINT8 *ChassisType
  )
{
  ASSERT (FALSE);
  *ChassisType = MiscChassisTypeUnknown;
  return EFI_SUCCESS;
}

/** Returns whether the specified processor is present or not.

  @param ProcessIndex The processor index to check.

  @return TRUE is the processor is present, FALSE otherwise.
**/
EFIAPI
BOOLEAN
OemIsSocketPresent (
  IN UINTN ProcessorIndex
  )
{
  ASSERT (FALSE);
  return FALSE;
}

/** Updates the HII string for the specified field.

  @param mHiiHandle    The HII handle.
  @param TokenToUpdate The string to update.
  @param Offset        The field to get information about.
**/
EFIAPI
VOID
OemUpdateSmbiosInfo (
  IN EFI_HII_HANDLE mHiiHandle,
  IN EFI_STRING_ID TokenToUpdate,
  IN OEM_MISC_SMBIOS_HII_STRING_FIELD Offset
  )
{
  ASSERT (FALSE);
}
