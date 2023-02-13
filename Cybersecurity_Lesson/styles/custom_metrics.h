#ifndef CUSTOM_METRICS_H
#define CUSTOM_METRICS_H

struct ScrollBar_Metrics
{
    // ScrollBar
    const int width = 39;
    const int padding = 8; // distance between handle and border (floating handle)

    // ScrollBar Handle
    const int handle_minimum = 30;
    const int handle_radius = 10;

    // TreeView
    int treeview_scrollbar_padding = 35;
    int header_height = 27;
};

// ComboBox
const int combobox_padding = 8;

#endif // CUSTOM_METRICS_H
