(function() {

	// Here we dynamically specify the content to load on
	// the page. The page loader takes that specification
	// and fulfills it.

	let content = [
		{
			template: "header",
			destination: "content-header",

			data: { type: "api-call", value: "/content/root/header.json" }
		},
		{
			template: "infoCardDiv",
			destination: "content-body",

			data: { type: "static", value: { title: "projects", html: '<a href="https://www.github.com/kevinclark19a" target="_blank">Projects</a>' } }
		},
		{
			template: "content",
			destination: "infoCardDiv__projects",

			data: { type: "api-call", value: "/content/root/projects/webserver.json" }
		},
		{
			template: "content",
			destination: "infoCardDiv__projects",

			data: { type: "api-call", value: "/content/root/projects/seac.json" }
		}
	];

	$(document).ready(() => {
		const pageLoader = LoaderService.PageLoader();

		pageLoader.boot(content).then((value) => {
			pageLoader.boot(content);

			if (value === true)
				console.log("Successfully booted index.html.");
			else
				console.log("An error occurred booting index.html. Please check previous logs.");
		});
	});
})()
