import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts

import MahyaProject

import "ComponentCreation.js" as MyScript

ApplicationWindow {
  id: applicationWindow

  minimumWidth: 640
  minimumHeight: 480
  visible: true
  title: "Mahya"

  Shortcut {
    sequences: [StandardKey.Close]
    onActivated: applicationWindow.close();
  }

  property int firstSavedQuote: 0
  property int lastSavedQuote: 0

  Backend {
    id: backend

    Component.onCompleted: MyScript.createErrorDialog();

    onErrorOccurred: (errorMessage) => {
      console.log("Main.qml,32: onErrorOccurred() was emitted.")
      MyScript.createErrorDialog();
    }

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

  MenuBar {
    id: menuBar

    Menu {
      id: fileMenu

      title: "File"

      MenuItem {
        id: fileOpenMenuItem

        text: "Open"
        onTriggered: console.log("Open action triggered");
      }

      MenuItem {
        id: fileQuitMenuItem

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
        Label {
          id: weekNumberLabel

          text: "Week #"
        }

        TextField {
          id: weekNumberField

          focus: true
          Layout.fillWidth: true
        }
      }

      RowLayout {
        Label {
          id: textLabel

          text: "Text"
        }

        ScrollView {
          id: textAreaView

          contentHeight: 100
          Layout.fillWidth: true

          TextArea {
            id: textArea

            // Set tab key to move to the next control.
            KeyNavigation.priority: KeyNavigation.BeforeItem
            KeyNavigation.tab: authorField
          }
        }

      }

      RowLayout {
        Label {
          text: "Author"
        }

        TextField {
          id: authorField

          Layout.fillWidth: true
        }
      }

      RowLayout {
        Label {
          text: "Author Description"
        }

        ScrollView {
          id: authorDescriptionAreaView

          contentHeight: 50
          Layout.fillWidth: true

          TextArea {
            id: authorDescriptionArea

            // Set tab key to move to the next control.
            KeyNavigation.priority: KeyNavigation.BeforeItem
            KeyNavigation.tab: textDescriptionArea
          }
        }
      }

      RowLayout {
        Label {
          id: textDescription

          text: "Text Description"
        }

        ScrollView {
          id: textDescriptionAreaView

          contentHeight: 50
          Layout.fillWidth: true

          TextArea {
            id: textDescriptionArea

            // Set tab key to move to the next control.
            KeyNavigation.priority: KeyNavigation.BeforeItem
            KeyNavigation.tab: saveButton
          }
        }
      }

      RowLayout {
        Layout.alignment: Qt.AlignRight

        Button {
          id: saveButton

          highlighted: true
          text: "Save"

          onClicked: {
            backend.saveQuote(
              weekNumberField.text,
              textArea.text,
              authorField.text,
              authorDescriptionArea.text,
              textDescriptionArea.text)

              backend.retrieveFirstLastQuotes()
            }
          }
        }
      }

      ColumnLayout {
        Label {
          id: firstSavedQuoteLabel

          text: "First saved quote: Week " + firstSavedQuote
        }

        Label {
          id: lastSavedQuoteLabel

          text: "Last saved quote: Week " + lastSavedQuote
        }

        Component.onCompleted: {
          backend.retrieveFirstLastQuotes()
        }
      }
    }
  }
