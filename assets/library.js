function mimeTypesToNames(list) {
	var humanList = [];
	for(var i = 0; i < list.length;i++) {
		switch(list[i]) {
			case "text/html":
			    humanList[i] = qsTr("HTML");
			    break;
			case "text/plain":
			    humanList[i] = qsTr("Plain text");
			    break;
			case "application/rtf":
			    humanList[i] = qsTr("Rich text");
			    break;
			case "application/vnd.oasis.opendocument.text":
			    humanList[i] = qsTr("Open Office doc");
			    break;
			case "application/pdf":
			    humanList[i] = qsTr("PDF");
			    break;
			case "application/vnd.openxmlformats-officedocument.wordprocessingml.document":
			    humanList[i] = qsTr("MS Word document");
			    break;
			case "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet":
			    humanList[i] = qsTr("MS Excel");
			    break;
			case "application/x-vnd.oasis.opendocument.spreadsheet":
			    humanList[i] = qsTr("Open Office sheet");
			    break;
			case "image/jpeg":
			    humanList[i] = qsTr("JPEG");
			    break;
			case "image/png":
			    humanList[i] = qsTr("PNG");
			    break;
			case "image/svg+xml":
			    humanList[i] = qsTr("SVG");
			    break;
			case "application/vnd.openxmlformats-officedocument.presentationml.presentation":
			    humanList[i] = qsTr("MS PowerPoint");
			    break;
			case "application/vnd.google-apps.script+json":
			    humanList[i] = qsTr("JSON");
			    break;
			case "text/csv":
			    humanList[i] = qsTr("CSV");
			    break;
		    default:
		    	humanList[i] = list[i];
		    	break;
		}
	}
	return humanList;
}

function humanFileSize(bytes) {
    var thresh = 1000;
    if(Math.abs(bytes) < thresh) {
        return bytes + ' B';
    }
    var units = [qsTr("kB"), qsTr("MB"), qsTr("GB"), qsTr("TB"), qsTr("PB"), qsTr("EB"), qsTr("ZB"), qsTr("YB")];
    var u = -1;
    do {
        bytes /= thresh;
        ++u;
    } while(Math.abs(bytes) >= thresh && u < units.length - 1);
    return bytes.toFixed(1)+' '+units[u];
}

function objectSize(obj) {
	var size = 0;
    for (var key in obj) {
        if (obj.hasOwnProperty(key)) size++;
    }
    return size;
}