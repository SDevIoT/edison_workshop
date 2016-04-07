(function () {
    var versionString = "0407.1601";
    var argv = process.argv;

    console.log("JN JJ SERVER " + versionString);
    var http = require('http'),
            fs = require('fs');
    var rootPath = "/home/root/node_modules/jn_jj_server/";
    var serverPort = 8124;

    console.log("ARGV:");
    console.log(argv);
    console.log(argv.length);

    console.log("Scanning Parameters...");
    for (var i = 0; i < argv.length; i++) {
        var parameter = process.argv[i];
        var paramKeyValue = parameter.split("=");
        if (paramKeyValue[0] === "path" && paramKeyValue[1]) {
            rootPath = paramKeyValue[1];
        }
        console.log(paramKeyValue);
    }
    console.log(rootPath);

    http.createServer(function (request, response) {
        console.log("REQUEST RECEIVED!");
        console.log(request.url);
        var path = rootPath + "/index.html";

        if (request.url !== "/MOVE") {
            if (request.url !== "/") {
                path = rootPath + request.url;
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
            fs.writeFileSync(rootPath + '/command.txt', command);
            console.log('File saved!');
        }

    }).listen(serverPort);

    console.log("Listening to port " + serverPort);
})();