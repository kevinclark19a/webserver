(function() {
	if (typeof LoaderService === "undefined") LoaderService = { };

	LoaderService.currentContent = LoaderService.currentContent || [];

	const boot = function(content) {
		const contentLoader = LoaderService.ContentLoader();

		const cleanPromises = Promise.all(LoaderService.currentContent.map( (elem) => contentLoader.clean(elem) ));
		const addPromises = Promise.all(content.map( (elem) => contentLoader.load(elem) ));

		LoaderService.currentContent = content.map( (elem) => elem.destination );

		return cleanPromises.then(() => addPromises.then(() => true, () => false));
	};

	LoaderService.PageLoader = function PageLoader() {
		return {
			boot: boot
		};
	};
})()
