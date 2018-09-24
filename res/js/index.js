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

	let projectsCard = {
		template: "infoCardDiv",
		destination: "content-body",

		data: { type: "static", value: { id: "projects", html: '<a href="https://www.github.com/kevinclark19a" target="_blank">Projects</a>' } }
	};

	let publicationsCard = {
		template: "infoCardDiv",
		destination: "content-body",

		data: { type: "static", value: { id: "publications", html: "Publications" } }
	};

	let dragOptions = {
		drag: function(event, ui) {
			$(".mirror").hide().show(0);
		}

	};

	let resizeOptions = {
		animate: true,
		animateDuration: 750,
		ghost: true,
		handles: {
			sw: "#swgrip",
			se: "#segrip",
		}
	};


	$(document).ready(() => {
		document.addEventListener("scroll", function() {
			$('.blur').hide().show(0);
		});

		const pageLoader = LoaderService.PageLoader();
		const contentLoader = LoaderService.ContentLoader();

		pageLoader.boot(content).then((value) => {
			$('.draggable').draggable(dragOptions);

			if (value === true)
				console.log("Successfully booted index.html.");
			else
				console.log("An error occurred booting index.html. Please check previous logs.");

			document.getElementById("header-link__projectsBtn").addEventListener("click", function() {

				if (document.getElementById("infoCardDiv__projects")) {
					contentLoader.remove("infoCardDiv__projects");
					return;
				}

				contentLoader.load(projectsCard).then(() => {
					$(".draggable").draggable(dragOptions);
					$(".resizable").resizable(resizeOptions);

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
					contentLoader.remove("infoCardDiv__publications");
					return;
				}

				contentLoader.load(publicationsCard).then(() => {
					$(".draggable").draggable(dragOptions);
					$(".resizable").resizable(resizeOptions);

					contentLoader.load({
						template: "content",
						destination: "infoCardDiv__publications",

						data: { type: "api-call", value: "/content/root/publications/acs.json" }
					});

					contentLoader.load({
						template: "content",
						destination: "infoCardDiv__publications",

						data: { type: "api-call", value: "/content/root/publications/isvc.json" }
					});
				});

			});

			document.getElementById("header-link__resumeBtn").addEventListener("click", function() {
				window.open("/content/resume.pdf");
			});

		});
	});
})()
