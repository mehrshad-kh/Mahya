var component;
var errorDialog;

function createErrorDialog() {
    console.log("ComponentCreation.js,5: createErrorDialog() was called.");
    component = Qt.createComponent("ErrorDialog.qml");
    if (component.status == Component.Ready) {
        console.log("ComponentCreation.js,8: component is Ready.")
        finishCreation();
    } else {
        component.statusChanged.connect(finishCreation);
    }
}

function finishCreation() {
    if (component.status == Component.Ready) {
        errorDialog = component.createObject(applicationWindow);
        if (errorDialog == null) {
            // Error Handling
            console.log("Error creating object.");
        }
    } else if (component.status == Component.Error) {
        // Erro Handling
        console.log("Error loading component: ", component.errorString());
    }
}
