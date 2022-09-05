@GUI::Widget {
    fill_with_background_color: true
    layout: @GUI::VerticalBoxLayout {
        margins: [4]
    }

    @GUI::TabWidget {
        @GUI::Widget {
            title: "General"
            layout: @GUI::VerticalBoxLayout {
                margins: [8, 12]
                spacing: 10
            }

            @GUI::Widget {
                fixed_height: 34
                layout: @GUI::HorizontalBoxLayout {
                    spacing: 20
                }

                @GUI::ImageWidget {
                    name: "icon"
                    fixed_width: 32
                    fixed_height: 32
                    bitmap: "/res/icons/32x32/app-keyboard-settings.png"
                }

                @GUI::Label {
                    name: "name"
                    text: ""
                    text_alignment: "CenterLeft"
                }
            }

            @GUI::HorizontalSeparator {}

            @GUI::Widget {
                fixed_height: 14
                layout: @GUI::HorizontalBoxLayout {
                    spacing: 12
                }

                @GUI::Label {
                    text: "Class:"
                    text_alignment: "CenterLeft"
                    fixed_width: 80
                }

                @GUI::Label {
                    name: "class"
                    text: ""
                    text_alignment: "CenterLeft"
                }
            }

            @GUI::Widget {
                fixed_height: 14
                layout: @GUI::HorizontalBoxLayout {
                    spacing: 12
                }

                @GUI::Label {
                    text: "Vendor:"
                    text_alignment: "CenterLeft"
                    fixed_width: 80
                }

                @GUI::Label {
                    name: "vendor"
                    text: ""
                    text_alignment: "CenterLeft"
                }
            }

            @GUI::Widget {
                fixed_height: 14
                layout: @GUI::HorizontalBoxLayout {
                    spacing: 12
                }

                @GUI::Label {
                    text: "Interface:"
                    text_alignment: "CenterLeft"
                    fixed_width: 80
                }

                @GUI::Label {
                    name: "programming_interface"
                    text: ""
                    text_alignment: "CenterLeft"
                }
            }

            @GUI::Widget {
                fixed_height: 14
                layout: @GUI::HorizontalBoxLayout {
                    spacing: 12
                }

                @GUI::Label {
                    text: "Location:"
                    text_alignment: "CenterLeft"
                    fixed_width: 80
                }

                @GUI::Label {
                    name: "location"
                    text: ""
                    text_alignment: "CenterLeft"
                }
            }

            @GUI::GroupBox {
                title: "Resources"
                layout: @GUI::VerticalBoxLayout {
                    margins: [8]
                }

                @GUI::TableView {
                    name: "resource-table-view"
                }
            }
        }
    }
}
