# tuya_deamon
## Usage
Build
```
make
```
Start
```
cd src
./tuya_daemon "Product ID" "Device ID" "Device Secret"
```
Clean
```
make clean
```
## Notes
ARGP leaks memory when running 0 arguments with built in options
