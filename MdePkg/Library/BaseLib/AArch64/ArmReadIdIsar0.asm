;------------------------------------------------------------------------------
;
; ArmReadIdIsar0() for AArch64
;
; Copyright (c) 2020, NUVIA Inc. All rights reserved.<BR>
;
; SPDX-License-Identifier: BSD-2-Clause-Patent
;
;------------------------------------------------------------------------------

  EXPORT ArmReadIdIsar0
  AREA BaseLib_LowLevel, CODE, READONLY

;/**
;  Reads the ID_AA64ISAR0 Register.
;
;**/
;UINT64
;EFIAPI
;ArmReadIdIsar0 (
;  VOID
;  );
;
ArmReadIdIsar0
  mrs  x0, id_aa64isar0_el1 // Read ID_AA64ISAR0 Register
  ret

  END
