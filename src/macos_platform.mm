#include "macos_platform.hpp"

#import <UniformTypeIdentifiers/UniformTypeIdentifiers.h>

#import <AppKit/AppKit.h>
#include <QGuiApplication>
#include <QWidget>

namespace macos {
    void apply_title_bar(QWidget& window, bool dark) {
        if (QGuiApplication::platformName() != "cocoa")
            return;

        auto*     native_view   = reinterpret_cast<NSView*>(window.winId());
        NSWindow* native_window = native_view.window;
        if (native_window == nil)
            return;

        native_window.styleMask |= NSWindowStyleMaskFullSizeContentView;
        native_window.titleVisibility            = NSWindowTitleHidden;
        native_window.titlebarAppearsTransparent = YES;
        native_window.titlebarSeparatorStyle     = NSTitlebarSeparatorStyleNone;
        native_window.backgroundColor            = dark ? [NSColor colorWithSRGBRed:15.0 / 255.0 green:23.0 / 255.0 blue:42.0 / 255.0 alpha:1.0] :
                                                          [NSColor colorWithSRGBRed:244.0 / 255.0 green:247.0 / 255.0 blue:251.0 / 255.0 alpha:1.0];
        native_window.appearance                 = [NSAppearance appearanceNamed:dark ? NSAppearanceNameDarkAqua : NSAppearanceNameAqua];
    }

    QString select_file(QWidget& parent, const QString& title, const QStringList& allowed_extensions) {
        if (QGuiApplication::platformName() != "cocoa")
            return {};

        // Ensure the parent has a native window before presenting the panel.
        static_cast<void>(parent.winId());

        NSOpenPanel* panel            = [NSOpenPanel openPanel];
        panel.title                   = title.toNSString();
        panel.canChooseFiles          = YES;
        panel.canChooseDirectories    = NO;
        panel.allowsMultipleSelection = NO;

        NSMutableArray<UTType*>* content_types = [NSMutableArray arrayWithCapacity:allowed_extensions.size()];
        for (const QString& extension : allowed_extensions) {
            UTType* type = [UTType typeWithFilenameExtension:extension.toNSString()];
            if (type != nil)
                [content_types addObject:type];
        }
        if (content_types.count > 0)
            panel.allowedContentTypes = content_types;

        if ([panel runModal] != NSModalResponseOK || panel.URL == nil)
            return {};
        return QString::fromNSString(panel.URL.path);
    }
}
