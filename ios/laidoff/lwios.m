//
//  lwios.m
//  laidoff
//
//  Created by 김거엽 on 2017. 4. 15..
//  Copyright © 2017년 KIMGEO YEOB. All rights reserved.
//
			
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <ImageIO/ImageIO.h>

#include "lwbitmapcontext.h"
#include "constants.h"
#include "lwmacro.h"
#include "sound.h"
#include "lwcontext.h"
#include <czmq.h>
#include "lwime.h"

static id app_delegate;

void set_app_delegate(id ad /*app_delegate*/) {
    app_delegate = ad;
}

void requestRemoteNotificationDeviceToken()
{
//    if (![[NSUserDefaults standardUserDefaults] objectForKey:@"HasSeenPopup"])
    {
        UIAlertView *alert = [[UIAlertView alloc]initWithTitle:@"푸시 알림 등록"
                                                       message:@"푸시 알림을 위한 장치 토큰을 등록할까요?"
                                                      delegate:app_delegate
                                             cancelButtonTitle:@"아니오"
                                             otherButtonTitles:@"예",
                              nil];
        alert.tag = 100;
        [alert show];
        [[NSUserDefaults standardUserDefaults] setValue:@"YES" forKey:@"HasSeenPopup"];
    }
}

void lw_start_text_input_activity(LWCONTEXT* pLwc) {
    pLwc->last_text_input_seq = lw_get_text_input_seq();
    UIAlertView * alert = [[UIAlertView alloc] initWithTitle:@"닉네임"
                                                     message:@"닉네임을 입력하세요."
                                                    delegate:app_delegate
                                           cancelButtonTitle:@"취소"
                                           otherButtonTitles:@"확인",
                           nil];
    alert.tag = 200;
    alert.alertViewStyle = UIAlertViewStylePlainTextInput;
    [alert show];
}

char * create_string_from_file(const char * filename) {
    
    NSString *pathname = [[NSBundle mainBundle] pathForResource:[NSString stringWithFormat:@"assets/%s", filename] ofType:@""];
    
    NSString *str = [NSString stringWithContentsOfFile:pathname encoding:NSUTF8StringEncoding error:nil];
    
    return strdup((char *)[str UTF8String]);
}

void release_string(char * d) {
    free((void*)d);
}

char* create_binary_from_file(const char* filename, size_t* size) {
    
    NSString *pathname = [[NSBundle mainBundle] pathForResource:[NSString stringWithFormat:@"assets/%s", filename] ofType:@""];
    
    NSData* data = [NSData dataWithContentsOfFile:pathname];

    *size = data.length;
    
    char* d = (char*)malloc(data.length);
    memcpy(d, [data bytes], data.length);

    return d;
}

void release_binary(char * d) {
    free(d);
}

void play_sound(LW_SOUND lws) {
    
}

void stop_sound(LW_SOUND lws) {
    
}

HRESULT init_ext_sound_lib() {
    return 0;
}

int request_get_today_played_count() {
    return 1;
}

int request_get_today_playing_limit_count() {
    return 1;
}

int request_get_highscore() {
    return 0;
}


CGImageRef CGImageRef_load(const char *filename) {
    NSString *pathname = [[NSBundle mainBundle] pathForResource:[NSString stringWithFormat:@"assets/%s", filename] ofType:@""];
    
    UIImage *someImage = [UIImage imageWithContentsOfFile:pathname];
    
    return [someImage CGImage];
}

unsigned char* CGImageRef_data(CGImageRef image, int* w, int* h) {
    NSInteger width = CGImageGetWidth(image);
    NSInteger height = CGImageGetHeight(image);
    unsigned char *data = (unsigned char*)calloc(1, width*height*4);
    
    CGContextRef context = CGBitmapContextCreate(data,
                                                 width, height,
                                                 8, width * 4,
                                                 CGImageGetColorSpace(image),
                                                 kCGImageAlphaPremultipliedLast);
    
    CGContextDrawImage(context,
                       CGRectMake(0.0, 0.0, (float)width, (float)height),
                       image);
    CGContextRelease(context);
    
    *w = (int)width;
    *h = (int)height;
    
    return data;
}

const unsigned char* load_png_ios(const char* filename, LWBITMAPCONTEXT* pBitmapContext) {
    
    CGImageRef r = CGImageRef_load(filename);
    unsigned char* d = 0;
    if (r) {
        d = CGImageRef_data(r, &pBitmapContext->width, &pBitmapContext->height);
    }
    
    pBitmapContext->lock = 0;
    pBitmapContext->data = (char*)d;
    
    return d;
}

void unload_png_ios(LWBITMAPCONTEXT* pBitmapContext) {
    free(pBitmapContext->data);
    pBitmapContext->data = 0;
}

void lw_app_quit(LWCONTEXT* pLwc)
{
    pLwc->quit_request = 1;
    zsock_wait(pLwc->logic_actor);
    //glfwSetWindowShouldClose(lw_get_window(pLwc), GLFW_TRUE);
}
