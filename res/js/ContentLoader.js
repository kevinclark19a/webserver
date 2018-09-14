(function() {
	if (typeof LoaderService == "undefined") LoaderService = { };

	const apply = function(html, destination) {

		console.log(html.trim());

		var element = document.createElement("div");
		element.innerHTML = html.trim();

		element.childNodes.forEach((elem) =>
			document.getElementById(destination).appendChild(elem));

	};

	const load = function(content) {
		return new Promise((resolve, reject) => {

			const successFunc = function(template) {
				template = Handlebars.compile(template);

				if (content.data.type === "static") {
					apply(template(content.data.value), content.destination);

					resolve();
				} else if (content.data.type === "api-call") {
					$.ajax({
						url: content.data.value,
						success: (data) => {
							apply(template(data), content.destination);

							resolve(true);
						},

						failure: (error) => {
							console.log(error);

							reject(error);
						}
					});
				} else reject("Data Type not covered.");
			};

			$.ajax({
				url: "/html/templates/" + content.template + ".html",
				success: successFunc,
				failure: reject
			});
		});
	};

	const clean = function(destination) {

		var element = document.getElementById(destination);

		if (!element) return;

		console.log(destination)
		console.log(element.childNodes);

		for (var node in element.childNodes.values()) {
			console.log(node);
			if (!node.classList || !node.classList.contains("permanent"))
				element.removeChild(node)
		};

	};

	LoaderService.ContentLoader = function ContentLoader() {
		return {
			load: load,
			clean: clean
		};
	};
})()
