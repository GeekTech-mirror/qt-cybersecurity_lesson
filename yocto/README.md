# Scripts

## mk2parts.sh
Create two minimum partitions for boot

```
Minimum required 2 partitions
Sectors are 512 bytes
0     : 4MB, no partition, MBR then empty
8192  : 64 MB, FAT partition, bootloader, kernel 
139264: 2GB+, linux partition, root filesystem
```
