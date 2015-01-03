/* ���: hidsdi.h
 * �����: Christian Starkjohann
 * �������: microsin.ru 
 * ���� ��������: 2006-02-02
 * ���������: 4
 * Copyright: (c) 2006-2008 by OBJECTIVE DEVELOPMENT Software GmbH
 * ��������: GNU GPL v2 (��. License.txt) ��� ������������� (CommercialLicense.txt)
 * �������: $Id: hidsdi.h 553 2008-04-17 19:00:20Z cs $
 */

/*
�������� ��������
���� ���� �������� ���� hidsdi.h �� Windows DDK. �� ������ ��������� ���� �
��������� ������� � ���� ��������� ��� ������ �������. ���� �� ������
Windows DDK ������ ����� ����� ��� ������, ������� ������������ ������ � MinGW, 
����������� ���� ���� ������ ������.
*/

#ifndef _HIDSDI_H
#define _HIDSDI_H

#include <pshpack4.h>

#include <ddk/hidusage.h>
#include <ddk/hidpi.h>

typedef struct{
    ULONG   Size;
    USHORT  VendorID;
    USHORT  ProductID;
    USHORT  VersionNumber;
}HIDD_ATTRIBUTES;

void __stdcall      HidD_GetHidGuid(OUT LPGUID hidGuid);

BOOLEAN __stdcall   HidD_GetAttributes(IN HANDLE device, OUT HIDD_ATTRIBUTES *attributes);

BOOLEAN __stdcall   HidD_GetManufacturerString(IN HANDLE device, OUT void *buffer, IN ULONG bufferLen);
BOOLEAN __stdcall   HidD_GetProductString(IN HANDLE device, OUT void *buffer, IN ULONG bufferLen);
BOOLEAN __stdcall   HidD_GetSerialNumberString(IN HANDLE device, OUT void *buffer, IN ULONG bufferLen);

BOOLEAN __stdcall   HidD_GetFeature(IN HANDLE device, OUT void *reportBuffer, IN ULONG bufferLen);
BOOLEAN __stdcall   HidD_SetFeature(IN HANDLE device, IN void *reportBuffer, IN ULONG bufferLen);

BOOLEAN __stdcall   HidD_GetNumInputBuffers(IN HANDLE device, OUT ULONG *numBuffers);
BOOLEAN __stdcall   HidD_SetNumInputBuffers(IN HANDLE device, OUT ULONG numBuffers);

#include <poppack.h>

#endif
