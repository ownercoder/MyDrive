import bb.cascades 1.3

Page {
    Container {
        ListView {
            id: accountList
            dataModel: ArrayDataModel {
                id: dataModel
            }
            listItemComponents: [
                ListItemComponent {
                    type: "user"
                    Container {
                        horizontalAlignment: HorizontalAlignment.Center
                        verticalAlignment: VerticalAlignment.Center
                        Container {
                            layout: AbsoluteLayout {
                            }
                            verticalAlignment: VerticalAlignment.Center
                            horizontalAlignment: HorizontalAlignment.Center
                            ImageView {
                                imageSource: "asset:///images/photo.jpg"
                                preferredWidth: ui.du(30)
                                preferredHeight: ui.du(30)
                            }
                            ImageView {
                                imageSource: "asset:///images/round-avatar.png"
                                preferredHeight: ui.du(30)
                                preferredWidth: ui.du(30)
                            }
                        } // end user avatar
                        Container {
                            layout: GridLayout {
                                columnCount: 1
                            }
                            horizontalAlignment: HorizontalAlignment.Center
                            Label {
                                text: "Сергей Квартников"
                                textStyle.fontWeight: FontWeight.Bold
                                verticalAlignment: VerticalAlignment.Center
                                horizontalAlignment: HorizontalAlignment.Center
                            }
                            Label {
                                text: "owner.coder@gmail.com"
                                textStyle.color: Color.create("#ff4e4e4e")
                                verticalAlignment: VerticalAlignment.Center
                                horizontalAlignment: HorizontalAlignment.Center
                            }
                        }
                    }
                },
                ListItemComponent {
                    type: "add"
                    Container {
                        topMargin: ui.du(10)
                        horizontalAlignment: HorizontalAlignment.Center
                        ImageView {
                            imageSource: "asset:///images/add-account.png"
                            preferredHeight: ui.du(30)
                            preferredWidth: ui.du(30)
                            horizontalAlignment: HorizontalAlignment.Center
                        }
                    }
                }
            ]
            layout: GridListLayout {
                columnCount: 1
                cellAspectRatio: 1.8

            }
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center

            topPadding: ui.du(2.0)
            function itemType(data, indexPath) {
                if (typeof accountList.dataModel.data(indexPath).addAccount != "undefined") {
                    return "add";
                }

                return "user";
            }
        }
        onCreationCompleted: {
            var arrayData = [];
            arrayData["picture480"] = "";
            dataModel.insert(0, Array("1"));
            var arrayData = [];
            arrayData["addAccount"] = true;
            dataModel.insert(0, Array(""));
            dataModel.insert(0, Array(""));
        }
    }
}
