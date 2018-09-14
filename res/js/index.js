(function() {

	// Here we dynamically specify the content to load on
	// the page. The page loader takes that specification
	// and fulfills it.

	let content = [
		{
			template: "header",
			destination: "content-header",

			data: { type: "api-call", value: "/content/root/header.json" }
		}
	];

	$(document).ready(() => {
		const pageLoader = LoaderService.PageLoader();
		const contentLoader = LoaderService.ContentLoader();

		pageLoader.boot(content).then((value) => {

			if (value === true)
				console.log("Successfully booted index.html.");
			else
				console.log("An error occurred booting index.html. Please check previous logs.");

			let projectsCard = {
				template: "infoCardDiv",
				destination: "content-body",

				data: { type: "static", value: { id: "projects", html: '<a href="https://www.github.com/kevinclark19a" target="_blank">Projects</a>' } }
			};

			let publicationsCard = {
				template: "infoCardDiv",
				destination: "content-body",

				data: { type: "static", value: { id: "publications", html: "<h1>Publications</h1>" } }
			};

			document.getElementById("header-link__projectsBtn").addEventListener("click", function() {

				if (document.getElementById("infoCardDiv__projects")) {
					contentLoader.clean("content-body");
					return;
				}

				contentLoader.clean("content-body");
				contentLoader.load(projectsCard).then(() => {
					$(".draggable").draggable();

					contentLoader.load({
						template: "content",
						destination: "infoCardDiv__projects",

						data: { type: "api-call", value: "/content/root/projects/webserver.json" }
					});

					contentLoader.load({
						template: "content",
						destination: "infoCardDiv__projects",

						data: { type: "api-call", value: "/content/root/projects/seac.json" }
					});

				});

			});

			document.getElementById("header-link__publicationsBtn").addEventListener("click", function() {
				if (document.getElementById("infoCardDiv__publications")) {
					contentLoader.clean("content-body");
					return;
				}

				contentLoader.clean("content-body");
				contentLoader.load(publicationsCard).then(() => {
					$(".draggable").draggable();

					contentLoader.load({
						template: "content",
						destination: "infoCardDiv__publications",

						data: { type: "api-call", value: "/content/root/publications/acs.json" }
					});

					contentLoader.load({
						template: "content",
						destination: "infoCardDiv__projects",

						data: { type: "api-call", value: "/content/root/publications/__.json" }
					});
				});

			});

			document.getElementById("header-link__resumeBtn").addEventListener("click", function() {
				if (document.getElementById("embeddedPDF__resume")) {
					contentLoader.clean("content-body");
					return;
				}
				contentLoader.clean("content-body");

				contentLoader.load({
					template: "pdf",
					destination: "content-body",

					data: { type: "static", value: { id: "resume", url: "/content/resume.pdf" } }
				});
			});

		});
	});
})()
