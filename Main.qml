import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Mahya

ApplicationWindow {
    minimumWidth: 640
    minimumHeight: 480
    visible: true
    title: "Mahya"


    Backend {
        id: my_backend

        onQuoteSaved: {
            week_number_field.text = ""
            text_area.text = ""
            author_field.text = ""
            author_description_area.text = ""
            text_description_area.text = ""
            week_number_field.focus = true
        }
    }

    menuBar: MenuBar {
        Menu {
            title: "File"
            MenuItem {
                text: "Open"
                onTriggered: console.log("Open action triggered");
            }
            MenuItem {
                text: "Quit"
                onTriggered: Qt.quit();
            }
        }
    }

    RowLayout {
        anchors.fill: parent

        ColumnLayout {
            RowLayout {
                // Layout.margins: 20

                    Layout.fillWidth: true
                Label {
                    text: "Week #"
                }

                TextField {
                    id: week_number_field

                }
            }

            RowLayout {
                // Layout.margins: 20

                Label {
                    id: text

                    text: "Text"
                }

                ScrollView {
                    id: text_area_view

                    contentHeight: 100
                    Layout.fillWidth: true

                    TextArea {
                        id: text_area
                    }
                }

            }

            RowLayout {
                // Layout.margins: 20

                Label {
                    text: "Author"
                }

                TextField {
                    id: author_field

                    Layout.fillWidth: true
                }
            }

            RowLayout {
                // Layout.margins: 20

                Label {
                    text: "Author Description"
                }

                ScrollView {
                    id: author_description_area_view

                    contentHeight: 50
                    Layout.fillWidth: true

                    TextArea {
                        id: author_description_area
                    }
                }
            }

            RowLayout {
                // Layout.margins: 20

                Label {
                    id: text_description

                    text: "Text Description"
                }

                ScrollView {
                    id: text_description_area_view

                    contentHeight: 50
                    Layout.fillWidth: true

                    TextArea {
                        id: text_description_area
                    }
                }
            }

            RowLayout {
                // Layout.margins: 20
                Layout.alignment: Qt.AlignRight

                Button {
                    id: save_button

                    text: "Save"

                    onClicked: {
                        my_backend.saveQuote(
                            week_number_field.text,
                            text_area.text,
                            author_field.text,
                            author_description_area.text,
                            text_description_area.text)
                    }
                }
            }
        }

        ColumnLayout {
            Label {
                text: "First saved quote: Week #" + 1
            }

            Label {
                text: "Last saved quote: Week #" + 10
            }
        }
    }
}
