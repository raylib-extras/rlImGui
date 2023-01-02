const c = @cImport({
    @cInclude("rlImGui.h");
    // @cInclude("rlImGuiColors.h");
});

pub fn setup(dark: bool) void {
    return c.rlImGuiSetup(dark);
}
pub fn shutdown() void {
    c.rlImGuiShutdown();
}
pub fn begin() void {
    c.rlImGuiBegin();
}
pub fn end() void {
    c.rlImGuiEnd();
}
