#ifndef LIB_PERMISSIONS
#define LIB_PERMISSIONS

#define USER_EXEC 0x100
#define USER_WRITE 0x200
#define USER_READ 0x400
#define USER_ALL 0x700

#define GROUP_EXEC 0x10
#define GROUP_WRITE 0x20
#define GROUP_READ 0x40
#define GROUP_ALL 0x70

#define OTHERS_EXEC 0x1
#define OTHERS_WRITE 0x2
#define OTHERS_READ 0x4
#define OTHERS_ALL 0x7

#endif