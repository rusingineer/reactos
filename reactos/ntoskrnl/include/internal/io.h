/*
* PROJECT:         ReactOS Kernel
* LICENSE:         GPL - See COPYING in the top level directory
* FILE:            ntoskrnl/include/io.h
* PURPOSE:         Internal header for the I/O Manager
* PROGRAMMERS:     Alex Ionescu (alex.ionescu@reactos.org)
*/
//#include "io_x.h"
#include "ntdddisk.h"

//
// Define this if you want debugging support
//
#define _IO_DEBUG_                                      0x00

//
// These define the Debug Masks Supported
//
#define IO_IRP_DEBUG                                    0x01

//
// Debug/Tracing support
//
#if _IO_DEBUG_
#ifdef NEW_DEBUG_SYSTEM_IMPLEMENTED // enable when Debug Filters are implemented
#define IOTRACE DbgPrintEx
#else
#define IOTRACE(x, ...)                                 \
    if (x & IopTraceLevel) DbgPrint(__VA_ARGS__)
#endif
#else
#define IOTRACE(x, ...) DPRINT(__VA_ARGS__)
#endif

//
// FIXME: What is this?
//
#define IO_METHOD_FROM_CTL_CODE(ctlCode)                (ctlCode&0x00000003)

//
// Packet Types when piggybacking on the IRP Overlay
//
#define IrpCompletionPacket                             0x1
#define IrpMiniCompletionPacket                         0x2

//
// Number of partition tables in the Boot Record
//
#define PARTITION_TBL_SIZE                              4

//
// Returns the size of a CM_RESOURCE_LIST
//
#define CM_RESOURCE_LIST_SIZE(ResList)                  \
    (ResList->Count == 1) ?                             \
        FIELD_OFFSET(                                   \
            CM_RESOURCE_LIST,                           \
            List[0].PartialResourceList.                \
            PartialDescriptors[(ResList)->              \
                               List[0].                 \
                               PartialResourceList.     \
                               Count])                  \
        :                                               \
        FIELD_OFFSET(CM_RESOURCE_LIST, List)

/*
 * VOID
 * IopDeviceNodeSetFlag(
 *   PDEVICE_NODE DeviceNode,
 *   ULONG Flag);
 */
#define IopDeviceNodeSetFlag(DeviceNode, Flag)          \
    ((DeviceNode)->Flags |= (Flag))

/*
 * VOID
 * IopDeviceNodeClearFlag(
 *   PDEVICE_NODE DeviceNode,
 *   ULONG Flag);
 */
#define IopDeviceNodeClearFlag(DeviceNode, Flag)        \
    ((DeviceNode)->Flags &= ~(Flag))

/*
 * BOOLEAN
 * IopDeviceNodeHasFlag(
 *   PDEVICE_NODE DeviceNode,
 *   ULONG Flag);
 */
#define IopDeviceNodeHasFlag(DeviceNode, Flag)          \
    (((DeviceNode)->Flags & (Flag)) > 0)

/*
 * VOID
 * IopDeviceNodeSetUserFlag(
 *   PDEVICE_NODE DeviceNode,
 *   ULONG UserFlag);
 */
#define IopDeviceNodeSetUserFlag(DeviceNode, UserFlag)  \
    ((DeviceNode)->UserFlags |= (UserFlag))

/*
 * VOID
 * IopDeviceNodeClearUserFlag(
 *   PDEVICE_NODE DeviceNode,
 *   ULONG UserFlag);
 */
#define IopDeviceNodeClearUserFlag(DeviceNode, UserFlag)\
    ((DeviceNode)->UserFlags &= ~(UserFlag))

/*
 * BOOLEAN
 * IopDeviceNodeHasUserFlag(
 *   PDEVICE_NODE DeviceNode,
 *   ULONG UserFlag);
 */
#define IopDeviceNodeHasUserFlag(DeviceNode, UserFlag)  \
    (((DeviceNode)->UserFlags & (UserFlag)) > 0)

 /*
 * VOID
 * IopDeviceNodeSetProblem(
 *   PDEVICE_NODE DeviceNode,
 *   ULONG Problem);
 */
#define IopDeviceNodeSetProblem(DeviceNode, Problem)    \
    ((DeviceNode)->Problem |= (Problem))

/*
 * VOID
 * IopDeviceNodeClearProblem(
 *   PDEVICE_NODE DeviceNode,
 *   ULONG Problem);
 */
#define IopDeviceNodeClearProblem(DeviceNode, Problem)  \
    ((DeviceNode)->Problem &= ~(Problem))

/*
 * BOOLEAN
 * IopDeviceNodeHasProblem(
 *   PDEVICE_NODE DeviceNode,
 *   ULONG Problem);
 */
#define IopDeviceNodeHasProblem(DeviceNode, Problem)    \
    (((DeviceNode)->Problem & (Problem)) > 0)

/*
 * VOID
 * IopInitDeviceTreeTraverseContext(
 *   PDEVICETREE_TRAVERSE_CONTEXT DeviceTreeTraverseContext,
 *   PDEVICE_NODE DeviceNode,
 *   DEVICETREE_TRAVERSE_ROUTINE Action,
 *   PVOID Context);
 */
#define IopInitDeviceTreeTraverseContext(               \
    _DeviceTreeTraverseContext, _DeviceNode, _Action,   \
    _Context) {                                         \
    (_DeviceTreeTraverseContext)->FirstDeviceNode =     \
        (_DeviceNode);                                  \
    (_DeviceTreeTraverseContext)->Action = (_Action);   \
    (_DeviceTreeTraverseContext)->Context = (_Context); }

//
// Special version of the IRP Overlay used to optimize I/O completion
// by not using up a separate structure.
//
typedef struct _IO_COMPLETION_PACKET
{
    struct
    {
        LIST_ENTRY ListEntry;
        union
        {
            struct _IO_STACK_LOCATION *CurrentStackLocation;
            ULONG PacketType;
        };
    };
    PVOID Key;
    PVOID Context;
    IO_STATUS_BLOCK IoStatus;
} IO_COMPLETION_PACKET, *PIO_COMPLETION_PACKET;

//
// Dummy File Object used inside the Open Packet so that OB knows how to
// deal with the Object Pointer even though it's not a real file.
//
typedef struct _DUMMY_FILE_OBJECT
{
    OBJECT_HEADER ObjectHeader;
    CHAR FileObjectBody[sizeof(FILE_OBJECT)];
} DUMMY_FILE_OBJECT, *PDUMMY_FILE_OBJECT;

//
// Open packet used as a context for Device/File parsing so that the parse
// routine can know what operation is being requested.
//
typedef struct _OPEN_PACKET
{
    CSHORT Type;
    CSHORT Size;
    PFILE_OBJECT FileObject;
    NTSTATUS FinalStatus;
    ULONG_PTR Information;
    ULONG ParseCheck;
    PFILE_OBJECT RelatedFileObject;
    OBJECT_ATTRIBUTES OriginalAttributes;
    LARGE_INTEGER AllocationSize;
    ULONG CreateOptions;
    USHORT FileAttributes;
    USHORT ShareAccess;
    PVOID EaBuffer;
    ULONG EaLength;
    ULONG Options;
    ULONG Disposition;
    PFILE_BASIC_INFORMATION BasicInformation;
    PFILE_NETWORK_OPEN_INFORMATION NetworkInformation;
    CREATE_FILE_TYPE CreateFileType;
    PVOID MailslotOrPipeParameters;
    BOOLEAN Override;
    BOOLEAN QueryOnly;
    BOOLEAN DeleteOnly;
    BOOLEAN FullAttributes;
    PDUMMY_FILE_OBJECT DummyFileObject;
    ULONG InternalFlags;
    //PIO_DRIVER_CREATE_CONTEXT DriverCreateContext; Vista only, needs ROS DDK Update
} OPEN_PACKET, *POPEN_PACKET;

//
// List of Bus Type GUIDs
//
typedef struct _IO_BUS_TYPE_GUID_LIST
{
    ULONG GuidCount;
    FAST_MUTEX Lock;
    GUID Guids[1];
} IO_BUS_TYPE_GUID_LIST, *PIO_BUS_TYPE_GUID_LIST;
extern PIO_BUS_TYPE_GUID_LIST IopBusTypeGuidList;

//
// Called on every visit of a node during a preorder-traversal of the device
// node tree.
// If the routine returns STATUS_UNSUCCESSFUL the traversal will stop and
// STATUS_SUCCESS is returned to the caller who initiated the tree traversal.
// Any other returned status code will be returned to the caller. If a status
// code that indicates an error (other than STATUS_UNSUCCESSFUL) is returned,
// the traversal is stopped immediately and the status code is returned to
// the caller.
//
typedef
NTSTATUS
(*DEVICETREE_TRAVERSE_ROUTINE)(
    IN PDEVICE_NODE DeviceNode,
    IN PVOID Context
);

//
// Context information for traversing the device tree
//
typedef struct _DEVICETREE_TRAVERSE_CONTEXT
{
    //
    // Current device node during a traversal
    //
    PDEVICE_NODE DeviceNode;

    //
    // Initial device node where we start the traversal
    //
    PDEVICE_NODE FirstDeviceNode;

    //
    // Action routine to be called for every device node
    //
    DEVICETREE_TRAVERSE_ROUTINE Action;

    //
    // Context passed to the action routine
    //
    PVOID Context;
} DEVICETREE_TRAVERSE_CONTEXT, *PDEVICETREE_TRAVERSE_CONTEXT;

//
// Internal Representation of a Disk
//
typedef struct _DISKENTRY
{
    LIST_ENTRY ListEntry;
    ULONG DiskNumber;
    ULONG Signature;
    ULONG Checksum;
    PDEVICE_OBJECT DeviceObject;
} DISKENTRY, *PDISKENTRY; 

//
// Partition Table Entry
//
#include <pshpack1.h>
typedef struct _PARTITION
{
    UCHAR BootFlags;
    UCHAR StartingHead;
    UCHAR StartingSector;
    UCHAR StartingCylinder;
    UCHAR PartitionType;
    UCHAR EndingHead;
    UCHAR EndingSector;
    UCHAR EndingCylinder;
    ULONG StartingBlock;
    ULONG SectorCount;
} PARTITION, *PPARTITION;

//
// Boot Record Structure
//
typedef struct _PARTITION_SECTOR
{
  UCHAR BootCode[440];
  ULONG Signature;
  UCHAR Reserved[2];
  PARTITION Partition[PARTITION_TBL_SIZE];
  USHORT Magic;
} PARTITION_SECTOR, *PPARTITION_SECTOR;
#include <poppack.h>

//
// PNP Routines
//
VOID
PnpInit(
    VOID
);

VOID
PnpInit2(
    VOID
);

VOID
IopInitDriverImplementation(
    VOID
);

VOID
IopInitPnpNotificationImplementation(
    VOID
);

VOID
IopNotifyPlugPlayNotification(
    IN PDEVICE_OBJECT DeviceObject,
    IN IO_NOTIFICATION_EVENT_CATEGORY EventCategory,
    IN LPCGUID Event,
    IN PVOID EventCategoryData1,
    IN PVOID EventCategoryData2
);

NTSTATUS
IopGetSystemPowerDeviceObject(
    IN PDEVICE_OBJECT *DeviceObject
);

NTSTATUS
IopCreateDeviceNode(
    IN PDEVICE_NODE ParentNode,
    IN PDEVICE_OBJECT PhysicalDeviceObject,
    OUT PDEVICE_NODE *DeviceNode
);

NTSTATUS
IopFreeDeviceNode(
    IN PDEVICE_NODE DeviceNode
);

NTSTATUS
IopInitiatePnpIrp(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIO_STATUS_BLOCK IoStatusBlock,
    IN ULONG MinorFunction,
    IN PIO_STACK_LOCATION Stack
);

PDEVICE_NODE
FASTCALL
IopGetDeviceNode(
    IN PDEVICE_OBJECT DeviceObject
);

NTSTATUS
IopActionConfigureChildServices(
    IN PDEVICE_NODE DeviceNode,
    IN PVOID Context
);

NTSTATUS
IopActionInitChildServices(
    IN PDEVICE_NODE DeviceNode,
    IN PVOID Context,
    IN BOOLEAN BootDrivers
);

NTSTATUS
IoCreateDriverList(
    VOID
);

NTSTATUS
IoDestroyDriverList(
    VOID
);

NTSTATUS 
INIT_FUNCTION
IopInitPlugPlayEvents(VOID);

NTSTATUS
IopQueueTargetDeviceEvent(
    const GUID *Guid,
    PUNICODE_STRING DeviceIds
);

NTSTATUS
IopInitializePnpServices(
    IN PDEVICE_NODE DeviceNode,
    IN BOOLEAN BootDrivers)
;

NTSTATUS
IopInvalidateDeviceRelations(
    IN PDEVICE_NODE DeviceNode,
    IN DEVICE_RELATION_TYPE Type
);

//
// Initialization Routines
//
NTSTATUS
IoCreateArcNames(
    VOID
);

NTSTATUS
IoCreateSystemRootLink(
    IN PCHAR ParameterLine
);

//
// Device/Volume Routines
//
NTSTATUS
FASTCALL
IopInitializeDevice(
    IN PDEVICE_NODE DeviceNode,
    IN PDRIVER_OBJECT DriverObject
);

NTSTATUS
IopStartDevice(
    IN PDEVICE_NODE DeviceNode
);

NTSTATUS
IoMountVolume(
    IN PDEVICE_OBJECT DeviceObject,
    IN BOOLEAN AllowRawMount
);

PVOID 
IoOpenSymlink(
    IN PVOID SymbolicLink
);

PVOID
IoOpenFileOnDevice(
    IN PVOID SymbolicLink,
    IN PWCHAR Name
);

NTSTATUS
STDCALL
IopAttachVpb(
    IN PDEVICE_OBJECT DeviceObject
);

VOID
IoInitFileSystemImplementation(
    VOID
);

VOID
IoInitVpbImplementation(
    VOID
);

//
// Shutdown routines
//
VOID
IoInitShutdownNotification(
    VOID
);

VOID
IoShutdownRegisteredDevices(
    VOID
);

VOID
IoShutdownRegisteredFileSystems(
    VOID
);

//
// Boot logging support
//
VOID
IopInitBootLog(
    IN BOOLEAN StartBootLog
);

VOID
IopStartBootLog(
    VOID
);

VOID
IopStopBootLog(
    VOID
);

VOID
IopBootLog(
    IN PUNICODE_STRING DriverName,
    IN BOOLEAN Success
);

VOID
IopSaveBootLogToFile(
    VOID
);

//
// I/O Cancellation Routines
//
VOID
STDCALL
IoCancelThreadIo(
    IN PETHREAD Thread
);

VOID
IoInitCancelHandling(
    VOID
);

//
// Error Logging Routines
//

NTSTATUS
IopInitErrorLog(
    VOID
);

//
// Raw File System MiniDriver
//
BOOLEAN
RawFsIsRawFileSystemDeviceObject(
    IN PDEVICE_OBJECT DeviceObject
);

NTSTATUS
STDCALL
RawFsDriverEntry(
    IN PDRIVER_OBJECT DriverObject,
    IN PUNICODE_STRING RegistryPath
);

//
// PnP Root MiniDriver
//
NTSTATUS
STDCALL
PnpRootDriverEntry(
   IN PDRIVER_OBJECT DriverObject,
   IN PUNICODE_STRING RegistryPath
);

NTSTATUS
PnpRootCreateDevice(
    IN OUT PDEVICE_OBJECT *PhysicalDeviceObject
);

//
// Driver Routines
//
VOID
FASTCALL
IopInitializeBootDrivers(
    VOID
);

VOID
FASTCALL
IopInitializeSystemDrivers(
    VOID
);

NTSTATUS
FASTCALL
IopCreateDriverObject(
    OUT PDRIVER_OBJECT *DriverObject,
    IN PUNICODE_STRING ServiceName,
    IN ULONG CreateAttributes,
    IN BOOLEAN FileSystemDriver,
    IN PVOID DriverImageStart,
    IN ULONG DriverImageSize
);

NTSTATUS
FASTCALL
IopGetDriverObject(
    OUT PDRIVER_OBJECT *DriverObject,
    IN PUNICODE_STRING ServiceName,
    IN BOOLEAN FileSystem
);

NTSTATUS
FASTCALL
IopLoadServiceModule(
    IN PUNICODE_STRING ServiceName,
    OUT PLDR_DATA_TABLE_ENTRY *ModuleObject
);

NTSTATUS 
FASTCALL
IopInitializeDriverModule(
    IN PDEVICE_NODE DeviceNode,
    IN PLDR_DATA_TABLE_ENTRY ModuleObject,
    IN PUNICODE_STRING ServiceName,
    IN BOOLEAN FileSystemDriver,
    OUT PDRIVER_OBJECT *DriverObject
);

NTSTATUS
FASTCALL
IopAttachFilterDrivers(
    IN PDEVICE_NODE DeviceNode,
    IN BOOLEAN Lower
);

VOID
FASTCALL
IopMarkLastReinitializeDriver(
    VOID
);

VOID
FASTCALL
IopReinitializeDrivers(
    VOID
);

//
// File Routines
//
NTSTATUS
NTAPI
IopParseDevice(
    IN PVOID ParseObject,
    IN PVOID ObjectType,
    IN OUT PACCESS_STATE AccessState,
    IN KPROCESSOR_MODE AccessMode,
    IN ULONG Attributes,
    IN OUT PUNICODE_STRING CompleteName,
    IN OUT PUNICODE_STRING RemainingName,
    IN OUT PVOID Context OPTIONAL,
    IN PSECURITY_QUALITY_OF_SERVICE SecurityQos OPTIONAL,
    OUT PVOID *Object
);

NTSTATUS
NTAPI
IopParseFile(
    IN PVOID ParseObject,
    IN PVOID ObjectType,
    IN OUT PACCESS_STATE AccessState,
    IN KPROCESSOR_MODE AccessMode,
    IN ULONG Attributes,
    IN OUT PUNICODE_STRING CompleteName,
    IN OUT PUNICODE_STRING RemainingName,
    IN OUT PVOID Context OPTIONAL,
    IN PSECURITY_QUALITY_OF_SERVICE SecurityQos OPTIONAL,
    OUT PVOID *Object
);

VOID
STDCALL
IopDeleteFile(
    IN PVOID ObjectBody
);

NTSTATUS
STDCALL
IopSecurityFile(
    IN PVOID ObjectBody,
    IN SECURITY_OPERATION_CODE OperationCode,
    IN SECURITY_INFORMATION SecurityInformation,
    IN OUT PSECURITY_DESCRIPTOR SecurityDescriptor,
    IN OUT PULONG BufferLength,
    OUT PSECURITY_DESCRIPTOR *OldSecurityDescriptor,
    IN POOL_TYPE PoolType,
    IN OUT PGENERIC_MAPPING GenericMapping
);

NTSTATUS
STDCALL
IopQueryNameFile(
    IN PVOID ObjectBody,
    IN BOOLEAN HasName,
    OUT POBJECT_NAME_INFORMATION ObjectNameInfo,
    IN ULONG Length,
    OUT PULONG ReturnLength,
    IN KPROCESSOR_MODE PreviousMode
);

VOID
STDCALL
IopCloseFile(
    IN PEPROCESS Process OPTIONAL,
    IN PVOID Object,
    IN ACCESS_MASK GrantedAccess,
    IN ULONG ProcessHandleCount,
    IN ULONG SystemHandleCount
);

//
// I/O Timer Routines
//
VOID
FASTCALL
IopInitTimerImplementation(
    VOID
);

VOID
STDCALL
IopRemoveTimerFromTimerList(
    IN PIO_TIMER Timer
);

//
// I/O Completion Routines
//
VOID
FASTCALL
IopInitIoCompletionImplementation(
    VOID
);

//
// I/O HAL Entrypoints
//
NTSTATUS
FASTCALL
xHalQueryDriveLayout(
    IN PUNICODE_STRING DeviceName,
    OUT PDRIVE_LAYOUT_INFORMATION *LayoutInfo
);

#undef HalExamineMBR
VOID 
FASTCALL
HalExamineMBR(
    IN PDEVICE_OBJECT DeviceObject,
    IN ULONG SectorSize,
    IN ULONG MBRTypeIdentifier,
    OUT PVOID *Buffer
);

VOID
FASTCALL
xHalIoAssignDriveLetters(
    IN PROS_LOADER_PARAMETER_BLOCK LoaderBlock,
    IN PSTRING NtDeviceName,
    OUT PUCHAR NtSystemPath,
    OUT PSTRING NtSystemPathString
);

NTSTATUS
FASTCALL
xHalIoReadPartitionTable(
    IN PDEVICE_OBJECT DeviceObject,
    IN ULONG SectorSize,
    IN BOOLEAN ReturnRecognizedPartitions,
    OUT PDRIVE_LAYOUT_INFORMATION *PartitionBuffer
);

NTSTATUS
FASTCALL
xHalIoSetPartitionInformation(
    IN PDEVICE_OBJECT DeviceObject,
    IN ULONG SectorSize,
    IN ULONG PartitionNumber,
    IN ULONG PartitionType
);

NTSTATUS
FASTCALL
xHalIoWritePartitionTable(
    IN PDEVICE_OBJECT DeviceObject,
    IN ULONG SectorSize,
    IN ULONG SectorsPerTrack,
    IN ULONG NumberOfHeads,
    IN PDRIVE_LAYOUT_INFORMATION PartitionBuffer
);

//
// Global I/O Data
//
extern POBJECT_TYPE IoCompletionType;
extern PDEVICE_NODE IopRootDeviceNode;
extern ULONG IopTraceLevel;

