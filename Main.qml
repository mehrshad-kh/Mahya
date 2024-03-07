import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Mahya

ApplicationWindow {
    minimumWidth: 640
    minimumHeight: 480
    visible: true
    title: "Mahya"

    property int firstSavedQuote: 0
    property int lastSavedQuote: 0

    Backend {
        id: myBackend

        onFirstSavedQuoteChanged: (value) => {
            firstSavedQuote = value
        }

        onLastSavedQuoteChanged: (value) => {
            lastSavedQuote = value
        }

        onQuoteSaved: {
            weekNumberField.text = ""
            textArea.text = ""
            authorField.text = ""
            authorDescriptionArea.text = ""
            textDescriptionArea.text = ""
            weekNumberField.focus = true
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
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        anchors.fill: parent

        ColumnLayout {
            RowLayout {
                // Layout.margins: 20

                Label {
                    text: "Week #"
                }

                TextField {
                    id: weekNumberField

                    Layout.fillWidth: true
                }
            }

            RowLayout {
                // Layout.margins: 20

                Label {
                    id: text

                    text: "Text"
                }

                ScrollView {
                    id: textArea_view

                    contentHeight: 100
                    Layout.fillWidth: true

                    TextArea {
                        id: textArea

                        // Set tab key to move to next control.
                        KeyNavigation.priority: KeyNavigation.BeforeItem
                        KeyNavigation.tab: authorField
                    }
                }

            }

            RowLayout {
                // Layout.margins: 20

                Label {
                    text: "Author"
                }

                TextField {
                    id: authorField

                    Layout.fillWidth: true
                }
            }

            RowLayout {
                // Layout.margins: 20

                Label {
                    text: "Author Description"
                }

                ScrollView {
                    id: authorDescriptionArea_view

                    contentHeight: 50
                    Layout.fillWidth: true

                    TextArea {
                        id: authorDescriptionArea

                        // Set tab key to move to next control.
                        KeyNavigation.priority: KeyNavigation.BeforeItem
                        KeyNavigation.tab: textDescriptionArea
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
                    id: textDescriptionArea_view

                    contentHeight: 50
                    Layout.fillWidth: true

                    TextArea {
                        id: textDescriptionArea

                        // Set tab key to move to next control.
                        KeyNavigation.priority: KeyNavigation.BeforeItem
                        KeyNavigation.tab: save_button
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
                        myBackend.saveQuote(
                            weekNumberField.text,
                            textArea.text,
                            authorField.text,
                            authorDescriptionArea.text,
                            textDescriptionArea.text)

                        myBackend.retrieveFirstLastQuotes()
                    }
                }
            }
        }

        ColumnLayout {
            Label {
                text: "First saved quote: Week " + firstSavedQuote
            }

            Label {
                text: "Last saved quote: Week " + lastSavedQuote
            }

            Component.onCompleted: {
                myBackend.retrieveFirstLastQuotes()
            }
        }
    }
}
