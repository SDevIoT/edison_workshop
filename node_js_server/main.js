var http = require('http'),
        fs = require('fs');
var serverPort = 8124;

console.log("VERSION 1912");
http.createServer(function (request, response) {
    console.log("REQUEST RECEIVED!");
    console.log(request.url);
    var path = "./main.html";

    if (request.url !== "/MOVE") {
        if (request.url !== "/") {
            path = "." + request.url;
        }

        console.log("PATH: " + path);
        var content = "";
        try {
            response.writeHeader(200, {"Content-Type": "text/html"});
            content = fs.readFileSync(path).toString();
            response.write(content);
            response.end();
        } catch (ex) {
            response.writeHeader(401, {"Content-Type": "text/html"});
            console.log("exception");
        }
    } else {
        console.log(request.url);
        response.writeHeader(200, {
            "Content-Type": "text/html",
            "Access-Control-Allow-Origin": "*"
        });
        var datetime = new Date();
        response.write("Command Received!  (" + datetime + ")");
        response.end();
        var command = "MOVE";
        fs.writeFileSync('./command.txt', command);
        console.log('File saved!');
    }

}).listen(serverPort);

console.log("Listening to port " + serverPort);
