#import <Cocoa/Cocoa.h>
#import <OpenGL/gl.h>
#import <OpenGL/glext.h>
#include <cstdio>

int main() {
  @autoreleasepool {
    NSOpenGLPixelFormatAttribute attrs[] = {
      NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersionLegacy,
      NSOpenGLPFAColorSize, 24, NSOpenGLPFAAlphaSize, 8,
      (NSOpenGLPixelFormatAttribute)0
    };
    NSOpenGLPixelFormat *format = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
    std::printf("pixel_format=%s\n", format ? "created" : "nil");
    if (!format) return 1;
    NSOpenGLContext *context = [[NSOpenGLContext alloc] initWithFormat:format shareContext:nil];
    std::printf("context=%s\n", context ? "created" : "nil");
    if (!context) return 2;
    [context makeCurrentContext];
    std::printf("version=%s\nrenderer=%s\nvendor=%s\n", glGetString(GL_VERSION), glGetString(GL_RENDERER), glGetString(GL_VENDOR));
    GLuint texture = 0, framebuffer = 0;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 4, 4, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glGenFramebuffersEXT(1, &framebuffer);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, texture, 0);
    GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    std::printf("framebuffer_status=0x%x\n", status);
    if (status != GL_FRAMEBUFFER_COMPLETE_EXT) return 3;
    glViewport(0, 0, 4, 4);
    glClearColor(17.0f / 255, 34.0f / 255, 51.0f / 255, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    unsigned char pixel[4] = {};
    glReadPixels(0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
    std::printf("clear_rgba=%u,%u,%u,%u\n", pixel[0], pixel[1], pixel[2], pixel[3]);
    bool clear_ok = pixel[0] == 17 && pixel[1] == 34 && pixel[2] == 51 && pixel[3] == 255;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor4ub(204, 102, 51, 255);
    glBegin(GL_QUADS);
    glVertex2f(-1, -1); glVertex2f(1, -1); glVertex2f(1, 1); glVertex2f(-1, 1);
    glEnd();
    glReadPixels(2, 2, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
    GLenum error = glGetError();
    std::printf("fixed_function_rgba=%u,%u,%u,%u\ngl_error=0x%x\n", pixel[0], pixel[1], pixel[2], pixel[3], error);
    bool fixed_ok = pixel[0] == 204 && pixel[1] == 102 && pixel[2] == 51 && pixel[3] == 255 && error == GL_NO_ERROR;
    glDeleteFramebuffersEXT(1, &framebuffer);
    glDeleteTextures(1, &texture);
    [NSOpenGLContext clearCurrentContext];
    std::printf("result=%s\n", clear_ok && fixed_ok ? "PASS" : "FAIL");
    return clear_ok && fixed_ok ? 0 : 4;
  }
}
