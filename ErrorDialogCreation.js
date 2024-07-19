var component;
var errorDialog;

function createErrorDialog(text, informativeText) {
    component = Qt.createComponent("ErrorDialog.qml");
    if (component.status == Component.Ready) {
        finishCreation(text, informativeText);
    } else {
        component.statusChanged.connect(finishCreation);
    }
}

function finishCreation(text, informativeText) {
    if (component.status == Component.Ready) {
        errorDialog = component.createObject(applicationWindow);
        if (errorDialog == null) {
            console.log("Error creating object.");
        }
        errorDialog.text = text
        errorDialog.informativeText = informativeText
        errorDialog.open();
    } else if (component.status == Component.Error) {
        console.log("Error loading component: ", component.errorString());
    }
}
