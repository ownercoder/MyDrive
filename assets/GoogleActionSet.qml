import bb.cascades 1.3
import bb.cascades.pickers 1.0
import com.slytech.apiwrapper 1.0
import com.slytech.googledownloader 1.0

ActionSet {
    title: ListItemData.title
    id: actionSet
    actions: [
        MultiSelectActionItem {
            multiSelectHandler: actionSet.parent.ListItem.view.multiSelectHandler
            title: qsTr("Select more")
        }
    ]
    attachedObjects: [
        ActionItem {
            id: copyAction
            title: qsTr("Copy to")
            onTriggered: {
                filePicker.mode = FilePickerMode.SaverMultiple;
                //filePicker.defaultSaveFileNames = ListItemData.title;
                filePicker.open();
            }
        },
        ActionItem {
            id: moveAction
            title: qsTr("Move to")
        },
        DeleteActionItem {
            id: removeAction
            title: qsTr("Remove")
            onTriggered: {
                actionSet.parent.parent.parent.trashFile(ListItemData);
            }
        },
        ActionItem {
            id: restoreAction
            title: qsTr("Restore")
        },
        DeleteActionItem {
            id: completelyRemoveAction
            title: qsTr("Delete forever")
        },
        ActionItem {
            id: addStarAction
            title: qsTr("Add star")
        },
        ActionItem {
            id: removeStarAction
            title: qsTr("Remove star")
        },
        ActionItem {
            id: shareAction
            title: qsTr("Share")
        },
        ActionItem {
            id: renameAction
            title: qsTr("Rename")
        },
        FilePicker {
            id: filePicker
            type: FileType.Other
            mode: FilePickerMode.SaverMultiple
            defaultType: FileType.Other
            onFileSelected: {
                actionSet.parent.parent.parent.downloadTo(selectedFiles[0], ListItemData);
            }
        }
    ]
    onCreationCompleted: {
        if (typeof ListItemData.labels.trashed && ListItemData.labels.trashed == true) {
            actionSet.add(restoreAction);
            actionSet.add(completelyRemoveAction);
            return;
        }
        
        actionSet.add(copyAction);
        actionSet.add(moveAction);
        actionSet.add(renameAction);
        if (typeof ListItemData.labels.starred && ListItemData.labels.starred == true) {
            actionSet.add(removeStarAction);
        } else {
            actionSet.add(addStarAction);
        }
        if (ListItemData.writersCanShare == true) {
            actionSet.add(shareAction);
        }
        actionSet.add(removeAction);
    }
}
