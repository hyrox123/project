/*
     File: RippleViewController.m
 Abstract: View controller that handles camera, drawing, and touch events.
  Version: 1.0
 
 Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple
 Inc. ("Apple") in consideration of your agreement to the following
 terms, and your use, installation, modification or redistribution of
 this Apple software constitutes acceptance of these terms.  If you do
 not agree with these terms, please do not use, install, modify or
 redistribute this Apple software.
 
 In consideration of your agreement to abide by the following terms, and
 subject to these terms, Apple grants you a personal, non-exclusive
 license, under Apple's copyrights in this original Apple software (the
 "Apple Software"), to use, reproduce, modify and redistribute the Apple
 Software, with or without modifications, in source and/or binary forms;
 provided that if you redistribute the Apple Software in its entirety and
 without modifications, you must retain this notice and the following
 text and disclaimers in all such redistributions of the Apple Software.
 Neither the name, trademarks, service marks or logos of Apple Inc. may
 be used to endorse or promote products derived from the Apple Software
 without specific prior written permission from Apple.  Except as
 expressly stated in this notice, no other rights or licenses, express or
 implied, are granted by Apple herein, including but not limited to any
 patent rights that may be infringed by your derivative works or by other
 works in which the Apple Software may be incorporated.
 
 The Apple Software is provided by Apple on an "AS IS" basis.  APPLE
 MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION
 THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS
 FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND
 OPERATION ALONE OR IN COMBINATION WITH YOUR PRODUCTS.
 
 IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL
 OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION,
 MODIFICATION AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED
 AND WHETHER UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE),
 STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 
 Copyright (C) 2012 Apple Inc. All Rights Reserved.
 
 */

#import <CoreVideo/CVOpenGLESTextureCache.h>
#import "RippleViewController.h"
#import "RippleModel.h"
#include "processInf.h"

// Uniform index.
enum
{
    UNIFORM_Y,
    UNIFORM_UV,
    NUM_UNIFORMS
};
GLint uniforms[NUM_UNIFORMS];

// Attribute index.
enum
{
    ATTRIB_VERTEX,
    ATTRIB_TEXCOORD,
    NUM_ATTRIBUTES
};

@interface RippleViewController () {
    GLuint _program;
    
    GLuint _positionVBO;
    GLuint _texcoordVBO;
    GLuint _indexVBO;
    
    CGFloat _screenWidth;
    CGFloat _screenHeight;
    size_t _textureWidth;
    size_t _textureHeight;
    unsigned int _meshFactor;
    
    EAGLContext *_context;
    RippleModel *_ripple;
    
    CVOpenGLESTextureRef _lumaTexture;
    CVOpenGLESTextureRef _chromaTexture;
    
    NSString *_sessionPreset;
    
    AVCaptureSession *_session;
    CVOpenGLESTextureCacheRef _videoTextureCache;    
}

- (void)cleanUpTextures;
- (void)setupAVCapture;
- (void)tearDownAVCapture;

- (void)setupBuffers;
- (void)setupGL;
- (void)tearDownGL;

- (BOOL)loadShaders;
- (BOOL)compileShader:(GLuint *)shader type:(GLenum)type file:(NSString *)file;
- (BOOL)linkProgram:(GLuint)prog;
@end

@implementation RippleViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    _context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];

    if (!_context) {
        NSLog(@"Failed to create ES context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = _context;
    self.preferredFramesPerSecond = 15;
    
    _screenWidth = [UIScreen mainScreen].bounds.size.width;
    _screenHeight = [UIScreen mainScreen].bounds.size.height;
    view.contentScaleFactor = [UIScreen mainScreen].scale;
    
    UITextField *edtCmuip = [[UITextField alloc] init];
    edtCmuip.delegate = self;
    edtCmuip.clearButtonMode = UITextFieldViewModeWhileEditing;
    edtCmuip.textAlignment = NSTextAlignmentLeft;
    edtCmuip.contentVerticalAlignment = UIControlContentVerticalAlignmentCenter;
    edtCmuip.tag = 50;
    edtCmuip.placeholder = @"CMU IP";
    edtCmuip.font = [UIFont systemFontOfSize:15];
    edtCmuip.frame = CGRectMake(20, self.view.frame.size.height - 280, self.view.frame.size.width / 2 - 10, 20);
    edtCmuip.backgroundColor = [UIColor clearColor];
    edtCmuip.returnKeyType = UIReturnKeyDone;
 
    edtCmuip.alpha = 0.9;
    [self.view addSubview:edtCmuip];
    

    UITextField *edtPuid = [[UITextField alloc] init];
    edtPuid.delegate = self;
    edtPuid.clearButtonMode = UITextFieldViewModeWhileEditing;
    edtPuid.textAlignment = NSTextAlignmentLeft;
    edtPuid.contentVerticalAlignment = UIControlContentVerticalAlignmentCenter;
    edtPuid.tag = 51;
    edtPuid.placeholder = @"PUID";
    edtPuid.font = [UIFont systemFontOfSize:15];
    edtPuid.frame = CGRectMake(self.view.frame.size.width / 2 + 20, self.view.frame.size.height - 280, self.view.frame.size.width / 2 - 20, 20);
    edtPuid.backgroundColor = [UIColor clearColor];
    edtPuid.returnKeyType = UIReturnKeyDone;

    edtPuid.alpha = 0.9;
    [self.view addSubview:edtPuid];

    UITextField *edtName = [[UITextField alloc] init];
    edtName.delegate = self;
    edtName.clearButtonMode = UITextFieldViewModeWhileEditing;
    edtName.textAlignment = NSTextAlignmentLeft;
    edtName.contentVerticalAlignment = UIControlContentVerticalAlignmentCenter;
    edtName.tag = 52;
    edtName.placeholder = @"Name";
    edtName.font = [UIFont systemFontOfSize:15];
    edtName.frame = CGRectMake(20, self.view.frame.size.height - 260, self.view.frame.size.width / 2 - 10, 20);
    edtName.backgroundColor = [UIColor clearColor];
    edtName.returnKeyType = UIReturnKeyDone;
    
    edtName.alpha = 0.9;
    [self.view addSubview:edtName];
    
    
    UITextField *edtPsw = [[UITextField alloc] init];
    edtPsw.delegate = self;
    edtPsw.clearButtonMode = UITextFieldViewModeWhileEditing;
    edtPsw.textAlignment = NSTextAlignmentLeft;
    edtPsw.contentVerticalAlignment = UIControlContentVerticalAlignmentCenter;
    edtPsw.tag = 53;
    edtPsw.placeholder = @"PassWord";
    edtPsw.font = [UIFont systemFontOfSize:15];
    edtPsw.frame = CGRectMake(self.view.frame.size.width / 2 + 20, self.view.frame.size.height - 260, self.view.frame.size.width / 2 - 20, 20);
    edtPsw.backgroundColor = [UIColor clearColor];
    edtPsw.returnKeyType = UIReturnKeyDone;
    
    edtPsw.alpha = 0.9;
    [self.view addSubview:edtPsw];

    UIButton *btn = [UIButton buttonWithType:UIButtonTypeCustom];
    btn.frame = CGRectMake(self.view.frame.size.width / 2 - 128, self.view.frame.size.height - 100, 256, 160);
    [btn setBackgroundImage:[UIImage imageNamed:@"btn1.png"] forState:UIControlStateNormal];
    [btn setBackgroundImage:[UIImage imageNamed:@"btn2.png"] forState:UIControlStateHighlighted];
    [btn setBackgroundImage:[UIImage imageNamed:@"btn3.png"] forState:UIControlStateSelected];
    [btn addTarget:self action:@selector(runPutask) forControlEvents:UIControlEventTouchDown];
    [btn setTitle:@"采集视频" forState:UIControlStateNormal];
    [btn setTitle:@"停止采集" forState:UIControlStateSelected];
    [btn setTitleColor:[UIColor colorWithWhite:0.10 alpha:1] forState:UIControlStateNormal];
    [btn setTitleColor:[UIColor colorWithWhite:0.90 alpha:1] forState:UIControlStateSelected];

    btn.tag = 100;
    btn.alpha = 0.5;
    [self.view addSubview:btn];
    
    UILabel *lable1 =[[UILabel alloc] initWithFrame:CGRectMake(5, 5, 320, 20)];
    lable1.textAlignment = NSTextAlignmentLeft;
    lable1.textColor = [UIColor colorWithWhite:0.9 alpha:1];
    lable1.font=[UIFont systemFontOfSize:16];
    lable1.backgroundColor=[UIColor clearColor];
    lable1.alpha = 0.5;
    lable1.tag = 200;
    [self.view addSubview:lable1];
    
    UILabel *lable2 =[[UILabel alloc] initWithFrame:CGRectMake(5, 30, 320, 20)];
    lable2.textAlignment = NSTextAlignmentLeft;
    lable2.textColor = [UIColor colorWithWhite:0.9 alpha:1];
    lable2.font=[UIFont systemFontOfSize:16];
    lable2.backgroundColor=[UIColor clearColor];
    lable2.alpha = 0.5;
    lable2.tag = 201;
    [self.view addSubview:lable2];

    UILabel *lable3 =[[UILabel alloc] initWithFrame:CGRectMake(5, 55, 320, 20)];
    lable3.textAlignment = NSTextAlignmentLeft;
    lable3.textColor = [UIColor colorWithWhite:0.9 alpha:1];
    lable3.font=[UIFont systemFontOfSize:16];
    lable3.backgroundColor=[UIColor clearColor];
    lable3.alpha = 0.5;
    lable3.tag = 202;
    [self.view addSubview:lable3];

    
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
    {
        // meshFactor controls the ending ripple mesh size.
        // For example mesh width = screenWidth / meshFactor.
        // It's chosen based on both screen resolution and device size.
        _meshFactor = 8;
        
        // Choosing bigger preset for bigger screen.
        _sessionPreset = AVCaptureSessionPreset1280x720;
    }
    else
    {
        _meshFactor = 4;
        _sessionPreset = AVCaptureSessionPresetLow;//AVCaptureSessionPreset640x480;
    }
    
    [self setupGL];
    [self setupAVCapture];
    
    _width = 0;
    _height = 0;
    _count = 0;
    _puTaskRun = NO;
    _mutex = [[NSCondition alloc] init];
}

- (void)viewDidUnload
{    
    [super viewDidUnload];
    
    [self tearDownAVCapture];
    
    [self tearDownGL];
    
    if ([EAGLContext currentContext] == _context) {
        [EAGLContext setCurrentContext:nil];
    }
}

- (void)runPutask
{
    UIButton * btn = (UIButton*)[self.view viewWithTag:100];
    UILabel *lable1 = (UILabel*)[self.view viewWithTag:200];
    UILabel *lable2 = (UILabel*)[self.view viewWithTag:201];
    UILabel *lable3 = (UILabel*)[self.view viewWithTag:202];
   
    UITextField *edtCMUIP = (UITextField *)[self.view viewWithTag:50];
    UITextField *edtPUID  = (UITextField *)[self.view viewWithTag:51];
    UITextField *edtUserName = (UITextField *)[self.view viewWithTag:52];
    UITextField *edtPsw  = (UITextField *)[self.view viewWithTag:53];

    
    if (!_puTaskRun)
    {
        edtCMUIP.hidden = YES;
        edtPUID.hidden = YES;
        edtUserName.hidden = YES;
        edtPsw.hidden = YES;
        
        _puTaskRun = YES;
        btn.selected = YES;
        PuRegisterInf puinf = {0};
        puinf.puid =  [[edtPUID text] UTF8String];//"1001_pusdk";
        puinf.name =  [[edtUserName text] UTF8String];//"pusdk";
        puinf.psw = [[edtPsw text] UTF8String];//"123456";
        puinf.ip = [[edtCMUIP text] UTF8String];///"192.168.2.145";
        puinf.port = 10001;
        puinf.company = 1;
        puinf.putype = 2;
        puinf.width = _width;
        puinf.height = _height;
        puinf.fps = self.preferredFramesPerSecond;
        registerPu(puinf);
        
        _fpsTimer = [NSTimer scheduledTimerWithTimeInterval:1.0
                                                               target:self
                                                             selector:@selector(refreshFps)
                                                             userInfo:nil
                                                              repeats:YES];

    }
    else
    {
        edtCMUIP.hidden = NO;
        edtPUID.hidden = NO;
        edtUserName.hidden = NO;
        edtPsw.hidden = NO;

        _puTaskRun = NO;
        btn.selected = NO;
        [_fpsTimer invalidate];
        lable1.text = @"";
        lable2.text = @"";
        lable3.text = @"";
        
        destoryPu();
    }
}


- (void)refreshFps
{
    [_mutex lock];
    int srcbitrate = _count / 1024;
    _count = 0;
    [_mutex unlock];
    
    int dstbitrate = getSendDataSize() / 1024;
    int fps = getSendFrameSize();
    UILabel *lable1 = (UILabel*)[self.view viewWithTag:200];
    UILabel *lable2 = (UILabel*)[self.view viewWithTag:201];
    UILabel *lable3 = (UILabel*)[self.view viewWithTag:202];
    
    NSString *message1 = [[NSString alloc] initWithFormat:@"视频大小:%dx%d", _width, _height];
    NSString *message2 = [[NSString alloc] initWithFormat:@"采集帧率:%d/S 发送帧率%d/S", self.preferredFramesPerSecond, fps];
    NSString *message3 = [[NSString alloc] initWithFormat:@"采集码率:%dKB/S 发送码率:%dKB/S", srcbitrate, dstbitrate];
    lable1.text = message1;
    lable2.text = message2;
    lable3.text = message3;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Release any cached data, images, etc. that aren't in use.
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Camera image orientation on screen is fixed 
    // with respect to the physical camera orientation.
    
    if (interfaceOrientation == UIInterfaceOrientationPortrait)
        return YES;
    else
        return NO;
}

- (void)cleanUpTextures
{    
    if (_lumaTexture)
    {
        CFRelease(_lumaTexture);
        _lumaTexture = NULL;        
    }
    
    if (_chromaTexture)
    {
        CFRelease(_chromaTexture);
        _chromaTexture = NULL;
    }
    
    // Periodic texture cache flush every frame
    CVOpenGLESTextureCacheFlush(_videoTextureCache, 0);
}

- (void)captureOutput:(AVCaptureOutput *)captureOutput 
didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer 
       fromConnection:(AVCaptureConnection *)connection
{
    CVReturn err;
	CVImageBufferRef pixelBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
    size_t width = CVPixelBufferGetWidth(pixelBuffer);
    size_t height = CVPixelBufferGetHeight(pixelBuffer);
    
    if (_width == 0 || _height == 0)
    {
        _width = width;
        _height = height;
    }

    if(CVPixelBufferLockBaseAddress(pixelBuffer, 0) == kCVReturnSuccess)
    {
        UInt8 *ptr = (UInt8 *)CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 0);
        size_t size = CVPixelBufferGetDataSize(pixelBuffer);
        onRecvRawData(ptr, size);
        
        [_mutex lock];
        _count += size;
        [_mutex unlock];
    }
    
    if (!_videoTextureCache)
    {
        NSLog(@"No video texture cache");
        return;
    }
    
    if (_ripple == nil ||
        width != _textureWidth ||
        height != _textureHeight)
    {
        _textureWidth = width;
        _textureHeight = height;
        
        _ripple = [[RippleModel alloc] initWithScreenWidth:_screenWidth 
                                              screenHeight:_screenHeight
                                                meshFactor:_meshFactor
                                               touchRadius:5
                                              textureWidth:_textureWidth
                                             textureHeight:_textureHeight];
        
        [self setupBuffers];
    }

    [self cleanUpTextures];
    
    // CVOpenGLESTextureCacheCreateTextureFromImage will create GLES texture
    // optimally from CVImageBufferRef.
    
    // Y-plane
    glActiveTexture(GL_TEXTURE0);
    err = CVOpenGLESTextureCacheCreateTextureFromImage(kCFAllocatorDefault, 
                                                       _videoTextureCache,
                                                       pixelBuffer,
                                                       NULL,
                                                       GL_TEXTURE_2D,
                                                       GL_RED_EXT,
                                                       _textureWidth,
                                                       _textureHeight,
                                                       GL_RED_EXT,
                                                       GL_UNSIGNED_BYTE,
                                                       0,
                                                       &_lumaTexture);
    if (err) 
    {
        NSLog(@"Error at CVOpenGLESTextureCacheCreateTextureFromImage %d", err);
    }   
    
    glBindTexture(CVOpenGLESTextureGetTarget(_lumaTexture), CVOpenGLESTextureGetName(_lumaTexture));
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
    
    // UV-plane
    glActiveTexture(GL_TEXTURE1);
    err = CVOpenGLESTextureCacheCreateTextureFromImage(kCFAllocatorDefault, 
                                                       _videoTextureCache,
                                                       pixelBuffer,
                                                       NULL,
                                                       GL_TEXTURE_2D,
                                                       GL_RG_EXT,
                                                       _textureWidth/2,
                                                       _textureHeight/2,
                                                       GL_RG_EXT,
                                                       GL_UNSIGNED_BYTE,
                                                       1,
                                                       &_chromaTexture);
    if (err) 
    {
        NSLog(@"Error at CVOpenGLESTextureCacheCreateTextureFromImage %d", err);
    }
    
    glBindTexture(CVOpenGLESTextureGetTarget(_chromaTexture), CVOpenGLESTextureGetName(_chromaTexture));
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

- (void)setupAVCapture
{
    //-- Create CVOpenGLESTextureCacheRef for optimal CVImageBufferRef to GLES texture conversion.
    CVReturn err = CVOpenGLESTextureCacheCreate(kCFAllocatorDefault, NULL, /*(__bridge void *)*/(EAGLContext*)_context, NULL, &_videoTextureCache);
    if (err) 
    {
        NSLog(@"Error at CVOpenGLESTextureCacheCreate %d", err);
        return;
    }
    
    //-- Setup Capture Session.
    _session = [[AVCaptureSession alloc] init];
    [_session beginConfiguration];
    
    //-- Set preset session size.
    [_session setSessionPreset:_sessionPreset];
    
    //-- Creata a video device and input from that Device.  Add the input to the capture session.
    AVCaptureDevice * videoDevice = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
    if(videoDevice == nil)
        assert(0);
    
    //-- Add the device to the session.
    NSError *error;        
    AVCaptureDeviceInput *input = [AVCaptureDeviceInput deviceInputWithDevice:videoDevice error:&error];
    if(error)
        assert(0);
    
    [_session addInput:input];
    
    //-- Create the output for the capture session.
    AVCaptureVideoDataOutput * dataOutput = [[AVCaptureVideoDataOutput alloc] init];
    [dataOutput setAlwaysDiscardsLateVideoFrames:YES]; // Probably want to set this to NO when recording
    
    //-- Set to YUV420.
    //kCVPixelFormatType_420YpCbCr8BiPlanarFullRange (real) kCVPixelFormatType_420YpCbCr8BiPlanarVideoRange
    
    [dataOutput setVideoSettings:[NSDictionary dictionaryWithObject:[NSNumber numberWithInt:kCVPixelFormatType_420YpCbCr8BiPlanarVideoRange] forKey:(id)kCVPixelBufferPixelFormatTypeKey]]; // Necessary for manual preview
    
    // Set dispatch to be on the main thread so OpenGL can do things with the data
    [dataOutput setSampleBufferDelegate:self queue:dispatch_get_main_queue()];            
    [_session addOutput:dataOutput];
    [_session commitConfiguration];
    
    [_session startRunning];
}

- (void)tearDownAVCapture
{
    [self cleanUpTextures];
    
    CFRelease(_videoTextureCache);
}

- (void)setupBuffers
{
    glGenBuffers(1, &_indexVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, [_ripple getIndexSize], [_ripple getIndices], GL_STATIC_DRAW);
    
    glGenBuffers(1, &_positionVBO);
    glBindBuffer(GL_ARRAY_BUFFER, _positionVBO);
    glBufferData(GL_ARRAY_BUFFER, [_ripple getVertexSize], [_ripple getVertices], GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(ATTRIB_VERTEX);
    glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);

    glGenBuffers(1, &_texcoordVBO);
    glBindBuffer(GL_ARRAY_BUFFER, _texcoordVBO);
    glBufferData(GL_ARRAY_BUFFER, [_ripple getVertexSize], [_ripple getTexCoords], GL_DYNAMIC_DRAW);
    
    glEnableVertexAttribArray(ATTRIB_TEXCOORD);
    glVertexAttribPointer(ATTRIB_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);
}

- (void)setupGL
{
    [EAGLContext setCurrentContext:_context];
    
    [self loadShaders];
    
    glUseProgram(_program);
        
    glUniform1i(uniforms[UNIFORM_Y], 0);
    glUniform1i(uniforms[UNIFORM_UV], 1);
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:_context];
    
    glDeleteBuffers(1, &_positionVBO);
    glDeleteBuffers(1, &_texcoordVBO);
    glDeleteBuffers(1, &_indexVBO);
    
    if (_program) {
        glDeleteProgram(_program);
        _program = 0;
    }
}

#pragma mark - GLKView and GLKViewController delegate methods

- (void)update
{
    if (_ripple)
    {
        [_ripple runSimulation];

        // no need to rebind GL_ARRAY_BUFFER to _texcoordVBO since it should be still be bound from setupBuffers
        glBufferData(GL_ARRAY_BUFFER, [_ripple getVertexSize], [_ripple getTexCoords], GL_DYNAMIC_DRAW);
    }
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    if (_ripple)
    {
        glDrawElements(GL_TRIANGLE_STRIP, [_ripple getIndexCount], GL_UNSIGNED_SHORT, 0);
    }
}

#pragma mark - Touch handling methods

- (void)myTouch:(NSSet *)touches withEvent:(UIEvent *)event
{
    for (UITouch *touch in touches) 
    {
        CGPoint location = [touch locationInView:touch.view]; 
        [_ripple initiateRippleAtLocation:location]; 
    }
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
 //   [self myTouch:touches withEvent:event];
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
 //   [self myTouch:touches withEvent:event];
}

#pragma mark - OpenGL ES 2 shader compilation

- (BOOL)loadShaders
{
    GLuint vertShader, fragShader;
    NSString *vertShaderPathname, *fragShaderPathname;
    
    // Create shader program.
    _program = glCreateProgram();
    
    // Create and compile vertex shader.
    vertShaderPathname = [[NSBundle mainBundle] pathForResource:@"Shader" ofType:@"vsh"];
    if (![self compileShader:&vertShader type:GL_VERTEX_SHADER file:vertShaderPathname]) {
        NSLog(@"Failed to compile vertex shader");
        return NO;
    }
    
    // Create and compile fragment shader.
    fragShaderPathname = [[NSBundle mainBundle] pathForResource:@"Shader" ofType:@"fsh"];
    if (![self compileShader:&fragShader type:GL_FRAGMENT_SHADER file:fragShaderPathname]) {
        NSLog(@"Failed to compile fragment shader");
        return NO;
    }
    
    // Attach vertex shader to program.
    glAttachShader(_program, vertShader);
    
    // Attach fragment shader to program.
    glAttachShader(_program, fragShader);
    
    // Bind attribute locations.
    // This needs to be done prior to linking.
    glBindAttribLocation(_program, ATTRIB_VERTEX, "position");
    glBindAttribLocation(_program, ATTRIB_TEXCOORD, "texCoord");
    
    // Link program.
    if (![self linkProgram:_program]) {
        NSLog(@"Failed to link program: %d", _program);
        
        if (vertShader) {
            glDeleteShader(vertShader);
            vertShader = 0;
        }
        if (fragShader) {
            glDeleteShader(fragShader);
            fragShader = 0;
        }
        if (_program) {
            glDeleteProgram(_program);
            _program = 0;
        }
        
        return NO;
    }
    
    // Get uniform locations.
    uniforms[UNIFORM_Y] = glGetUniformLocation(_program, "SamplerY");
    uniforms[UNIFORM_UV] = glGetUniformLocation(_program, "SamplerUV");
    
    // Release vertex and fragment shaders.
    if (vertShader) {
        glDetachShader(_program, vertShader);
        glDeleteShader(vertShader);
    }
    if (fragShader) {
        glDetachShader(_program, fragShader);
        glDeleteShader(fragShader);
    }
    
    return YES;
}

- (BOOL)compileShader:(GLuint *)shader type:(GLenum)type file:(NSString *)file
{
    GLint status;
    const GLchar *source;
    
    source = (GLchar *)[[NSString stringWithContentsOfFile:file encoding:NSUTF8StringEncoding error:nil] UTF8String];
    if (!source) {
        NSLog(@"Failed to load vertex shader");
        return NO;
    }
    
    *shader = glCreateShader(type);
    glShaderSource(*shader, 1, &source, NULL);
    glCompileShader(*shader);
    
#if defined(DEBUG)
    GLint logLength;
    glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetShaderInfoLog(*shader, logLength, &logLength, log);
        NSLog(@"Shader compile log:\n%s", log);
        free(log);
    }
#endif
    
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
    if (status == 0) {
        glDeleteShader(*shader);
        return NO;
    }
    
    return YES;
}

- (BOOL)linkProgram:(GLuint)prog
{
    GLint status;
    glLinkProgram(prog);
    
#if defined(DEBUG)
    GLint logLength;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(prog, logLength, &logLength, log);
        NSLog(@"Program link log:\n%s", log);
        free(log);
    }
#endif
    
    glGetProgramiv(prog, GL_LINK_STATUS, &status);
    if (status == 0) {
        return NO;
    }
    
    return YES;
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField {
    [textField resignFirstResponder];
    return YES;
}

@end
