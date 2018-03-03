#ifndef LIBNDGPP_DETAIL_VARIANT_STORAGE_HPP
#define LIBNDGPP_DETAIL_VARIANT_STORAGE_HPP

#include <type_traits>

namespace ndgpp
{
    namespace detail
    {
        class variant_storage_base
        {
            public:

            virtual ~variant_storage_base() {}

            virtual void copy_construct(void * storage) const {};
            virtual void move_construct(void * storage) {};

            virtual void move_assign(void * storage) {};
            virtual void copy_assign(void * storage) const {};

            virtual void * get_ptr() noexcept { return nullptr; };
            virtual void const * get_ptr() const noexcept { return nullptr; };
        };

        template <class S, class T>
        struct construct_noop
        {
            static void copy(const T& other, void * storage)
            {}

            static void move(T&& other, void * storage)
            {}
        };

        template <class S, class T>
        struct construct
        {
            static void copy(const T& other, void * storage)
            {
                new (storage) S(other);
            }

            static void move(T&& other, void * storage)
            {
                new (storage) S(std::move(other));
            }
        };

        template <class T>
        struct assign_noop
        {
            static void move(const T& other, void * storage)
            {}
        };

        template <class T>
        struct assign
        {
            static void move(T&& other, void * storage)
            {
                auto & t = *static_cast<T*>(storage);
                t = std::move(other);
            }

            static void copy(const T& other, void * storage)
            {
                auto & t = *static_cast<T*>(storage);
                t = other;
            }
        };

        template <class T>
        class variant_storage: public variant_storage_base
        {
            public:

            using value_type = std::decay_t<T>;

            variant_storage(const value_type& v);
            variant_storage(value_type&& v);

            ~variant_storage();

            void copy_construct(void * storage) const override;
            void move_construct(void * storage) override;

            void move_assign(void * storage) override;
            void copy_assign(void * storage) const override;

            const T& get() const & noexcept;
            T& get() & noexcept;
            T&& get() && noexcept;

            void * get_ptr() noexcept override;
            void const * get_ptr() const noexcept override;

            private:

            std::aligned_storage_t<sizeof(value_type), std::alignment_of<value_type>::value> storage_;
        };

        template <class T>
        inline variant_storage<T>::variant_storage(const value_type& v)
        {
            new (std::addressof(storage_)) value_type(v);
        }

        template <class T>
        inline variant_storage<T>::variant_storage(value_type&& v)
        {
            new (std::addressof(storage_)) value_type(std::move(v));
        }

        template <class T>
        inline variant_storage<T>::~variant_storage()
        {
            this->get().~value_type();
        }

        template <class T>
        inline const T& variant_storage<T>::get() const & noexcept
        {
            return *reinterpret_cast<value_type const *>(std::addressof(storage_));
        }

        template <class T>
        inline T& variant_storage<T>::get() & noexcept
        {
            return *reinterpret_cast<value_type *>(std::addressof(storage_));
        }

        template <class T>
        inline T&& variant_storage<T>::get() && noexcept
        {
            return std::move(*reinterpret_cast<value_type *>(std::addressof(storage_)));
        }

        template <class T>
        inline void * variant_storage<T>::get_ptr() noexcept
        {
            return reinterpret_cast<void *>(std::addressof(storage_));
        }

        template <class T>
        inline void const * variant_storage<T>::get_ptr() const noexcept
        {
            return reinterpret_cast<void const *>(std::addressof(storage_));
        }

        template <class T>
        inline void variant_storage<T>::copy_construct(void * storage) const
        {
            using constructor = std::conditional_t<std::is_copy_constructible<value_type>::value,
                                                   detail::construct<variant_storage<T>, T>,
                                                   detail::construct_noop<variant_storage<T>, T>>;
            constructor::copy(this->get(), storage);
        }

        template <class T>
        inline void variant_storage<T>::move_construct(void * storage)
        {
            using constructor = std::conditional_t<std::is_move_constructible<value_type>::value,
                                                   detail::construct<variant_storage<T>, T>,
                                                   detail::construct_noop<variant_storage<T>, T>>;
            constructor::move(std::move(this->get()), storage);
        }

        template <class T>
        inline void variant_storage<T>::move_assign(void * storage)
        {
            using assigner = std::conditional_t<std::is_copy_assignable<value_type>::value,
                                                detail::assign<T>,
                                                detail::assign_noop<T>>;

            assigner::move(std::move(this->get()), storage);
        }

        template <class T>
        inline void variant_storage<T>::copy_assign(void * storage) const
        {
            using assigner = std::conditional_t<std::is_copy_assignable<value_type>::value,
                                                detail::assign<T>,
                                                detail::assign_noop<T>>;

            assigner::copy(this->get(), storage);
        }
    }
}

#endif
