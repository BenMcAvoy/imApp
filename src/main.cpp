#include <spdlog/spdlog.h>

#include <sol/sol.hpp>

int fib(int n) {
	if (n < 2) {
		return n;
	} else {
		return fib(n - 1) + fib(n - 2);
	}
}

void exec(sol::state &state, const std::string &code) {
	auto res = state.safe_script(code, sol::script_pass_on_error);

	if (!res.valid()) {
		spdlog::error("Error: {}", res.get<sol::error>().what());
	}
}

int main(void) {
	sol::state lua;
	// enable jit
	lua.open_libraries(sol::lib::base, sol::lib::jit, sol::lib::os, sol::lib::math);
	lua["jit"]["on"]();

	lua.set_function("print", [](const std::string &str) {
		spdlog::info("{}", str);
	});

	auto bench = R"(
		jit.off()
		local function fib(n)
			if n < 2 then
				return n
			else
				return fib(n - 1) + fib(n - 2)
			end
		end

		print(tostring(fib(30)))
	)";

	std::vector<int> msTimings = {};

	for (int i = 0; i < 100; i++) {
		auto begin = std::chrono::steady_clock::now();
		exec(lua, bench);
		auto end = std::chrono::steady_clock::now();
		spdlog::info("Time (ms): {}", std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());

		msTimings.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());
	}

	int sum = 0;
	for (auto &ms : msTimings) {
		sum += ms;
	}

	spdlog::info("Average time (ms): {}", sum / msTimings.size());

	spdlog::info("Now testing C++ approach");

	std::vector<int> msTimingsCpp = {};

	for (int i = 0; i < 100; i++) {
		auto begin = std::chrono::steady_clock::now();
		spdlog::info("{}", fib(30));
		auto end = std::chrono::steady_clock::now();
		spdlog::info("Time (ms): {}", std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());

		msTimingsCpp.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());
	}

	int sumCpp = 0;
	for (auto &ms : msTimingsCpp) {
		sumCpp += ms;
	}

	spdlog::info("Average time (ms): {}", sumCpp / msTimingsCpp.size());
}
