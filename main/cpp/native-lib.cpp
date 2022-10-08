#include <jni.h>
#include <string>

#include <stdio.h>


#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}


std::string USB_GetSerialNumber()
{
    return exec("su -c cat /config/usb_gadget/g1/strings/0x409/serialnumber");
}

std::string USB_GetProduct()
{
    return exec("su -c cat /config/usb_gadget/g1/strings/0x409/product");
}


extern "C" JNIEXPORT jstring JNICALL
Java_com_example_usbsn_MainActivity_getSerialNumber(
        JNIEnv* env,
        jobject /* this */) {


    return env->NewStringUTF(    USB_GetSerialNumber().c_str()      );
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_usbsn_MainActivity_getProduct(
        JNIEnv* env,
        jobject /* this */) {


    return env->NewStringUTF(    USB_GetProduct().c_str()      );
}

std::string jstring2string(JNIEnv *env, jstring jStr){
    const char *cstr = env->GetStringUTFChars(jStr, NULL);
    std::string str = std::string(cstr);
    env->ReleaseStringUTFChars(jStr, cstr);
    return str;
}

#include <android/log.h>
#define LOG_TAG "dbg"
#define LOG(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)


extern "C" JNIEXPORT jint JNICALL
Java_com_example_usbsn_MainActivity_Patch(
        JNIEnv* env,
        jobject /* this */,

        jstring product, jstring serial


        ) {

    std::string c_product = jstring2string(env, product);
    std::string c_serial = jstring2string(env, serial);



    if (!strchr(c_product.c_str(), '\n'))
        c_product.erase(std::remove(c_product.begin(), c_product.end(), '\n'), c_product.end());


    if (!strchr(c_serial.c_str(), '\n'))
        c_serial.erase(std::remove(c_serial.begin(), c_serial.end(), '\n'), c_serial.end());

    std::string final_serial = "su -c \"echo ";
    final_serial += c_serial;
    final_serial += " > /config/usb_gadget/g1/strings/0x409/serialnumber\"";
    exec(final_serial.c_str());

    std::string final_product = "su -c \"echo " + c_product;
    final_product += " > /config/usb_gadget/g1/strings/0x409/product\"";
    exec(final_product.c_str());

    // exec("su -c \"echo > /config/usb_gadget/g1/strings/0x409/serialnumber\"");
    // exec("su -c \"echo USB > /config/usb_gadget/g1/strings/0x409/product\"");

    return 0;
}
