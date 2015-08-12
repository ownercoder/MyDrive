import bb.cascades 1.3
import bb.device 1.3

Page {
    ListView {
        id: listViewRoot
        dataModel: XmlDataModel {
            id: xmlDataModel
            source: "asset:///model.xml"
        }
        flickMode: FlickMode.None
        listItemComponents: [
            ListItemComponent {
                type: "listItem"
                Container {
                    Label {
                        text: ListItemData.title
                    }
                    background: Color.create(ListItemData.color)
                    preferredHeight: displayInfo.pixelSize.height
                    preferredWidth: displayInfo.pixelSize.width
                    attachedObjects: [
                        DisplayInfo {
                            id: displayInfo
                        }
                    ]
                }
            }
        ]
        function itemType(data, indexPath) {
            return "listItem";
        }
        scrollIndicatorMode: ScrollIndicatorMode.None
        layout: FlowListLayout {
            orientation: LayoutOrientation.LeftToRight

        }
        snapMode: SnapMode.LeadingEdge
    }
}