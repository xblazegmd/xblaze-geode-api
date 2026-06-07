#include <Geode/Geode.hpp>

namespace xblazeapi {
	class RobArray final {
	public:
		RobMap(std::string_view res, std::string_view sep) {
			const size_t count = std::count(res.begin(), res.end(), sep);
			m_values.reserve(count);

			const
		}

		// Get a value from the array by its key.
		std::string&& operator[](uint8_t key) {
			return std::move(m_values[key]);
		}
	private:
		std::vector<std::string> m_values;
	};
}