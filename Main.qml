import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Mahya

ApplicationWindow {
    width: 640
    height: 480
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

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Label {
                text: "Week #"
            }

            TextField {
                id: week_number_field

                Layout.fillWidth: true
            }
        }

        RowLayout {
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
            Label {
                text: "Author"
            }

            TextField {
                id: author_field

                Layout.fillWidth: true
            }
        }

        RowLayout {
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

}
