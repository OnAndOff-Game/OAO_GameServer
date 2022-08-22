#pragma once

#include <memory>

namespace OAO {
	namespace Util {
		template <typename TElement>
		class SharedArray {
		private:
			template <typename TPointer>
			struct ArrayDeleter {
				void operator()(TPointer* p) 
				{
					delete[] p;
				}
			};

			std::shared_ptr<TElement> m_ptr;

		public:
			explicit SharedArray(TElement *p = nullptr): m_ptr{p, ArrayDeleter<TElement>{}} {}
			SharedArray(const SharedArray &r) : m_ptr{r.m_ptr} {}

			void Reset(TElement* p = nullptr) { m_ptr.reset(p); }
			TElement operator[](std::ptrdiff_t i) const { return m_ptr.get()[i]; }
			TElement* get() const { return m_ptr.get(); }
			operator bool() const { return m_ptr.get() != nullptr; }
		};
	}
}