# Kernel Trap Latency Test
To evaluate trapping latency from user space to kernel

## Usage
```
cd kernelModule
make
./install
cd ..
cd userApp
make
./userApp $NrWrite $Direct
```
$NrWrite is the number of writes
$Direct==1 means write operations dont' trap into kernel
$Direct==0 means write operations will trap into kernel
