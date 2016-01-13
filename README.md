# PS4-Eye-Driver

This is a driver for the PlayStation 4 camera. 

To use this code on Windows you need a version on libusb with isocrhonous support. In order to do it download [libusb master branch](https://github.com/libusb) and merge it with [this other](https://github.com/JoshBlake/libusbx/tree/winiso).

```
git clone https://github.com/libusb/libusb.git libusb
cd libusb
git remote add libusbx https://github.com/JoshBlake/libusbx.git
git fetch libusbx
git merge libusbx/winiso
```

Also remember to set the devices composite parent controller to libusbK using [Zadig](http://zadig.akeo.ie/). To achieve it, once the firmware has been uploaded, open Zadig, click on "List All Devices" and unmark "Ignore Hubs or Composite Parents", then select the device's composite parent and choose the libusbK driver.
