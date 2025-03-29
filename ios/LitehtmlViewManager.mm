#import <React/RCTViewManager.h>
#import <React/RCTUIManager.h>
#import "RCTBridge.h"

@interface LitehtmlViewManager : RCTViewManager
@end

@implementation LitehtmlViewManager

RCT_EXPORT_MODULE(LitehtmlView)

- (UIView *)view
{
  return [[UIView alloc] init];
}

RCT_EXPORT_VIEW_PROPERTY(color, NSString)

@end
