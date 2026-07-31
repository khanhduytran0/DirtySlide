//
//  ViewController.m
//  DirtySlide
//
//  Created by duy on 7/4/25.
//

@import Darwin;
#import "ViewController.h"
#import "PrivateHeaders.h"

@interface ViewController ()
@property(nonatomic) NSDictionary<NSString*, void (^)(void)> *actions;
@end
@implementation ViewController

- (void)loadView {
    [super loadView];
    self.actions = @{
        @"Crash in dsc region": ^{
            uint32_t *crash_buf = (uint32_t *)getpid;
            *crash_buf = 0;
        },
        @"dyld": ^{
            int (*CVE_2026_dyld)(void) = dlsym(RTLD_DEFAULT, "CVE_2026_dyld");
            CVE_2026_dyld();
        },
    };
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 1;
}
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return self.actions.count;
}
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    static NSString *cellIdentifier = @"ActionCell";
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:cellIdentifier];
    if (!cell) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:cellIdentifier];
    }
    cell.textLabel.text = self.actions.allKeys[indexPath.row];
    return cell;
}
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    [tableView deselectRowAtIndexPath:indexPath animated:YES];
    void (^action)(void) = self.actions[self.actions.allKeys[indexPath.row]];
    if (action) {
        action();
    }
}

@end
