#pragma once


#include <cstdint>
#include <limits>

#include "config.h"
#include "list.h"


namespace siren {

class Scheduler;
namespace detail { struct SemaphoreWaiter; }


class Semaphore final
{
public:
    explicit Semaphore(Scheduler *, std::intmax_t = 0, std::intmax_t = 0
                       , std::intmax_t = std::numeric_limits<std::intmax_t>::max()) noexcept;
    Semaphore(Semaphore &&) noexcept;
    ~Semaphore();
    Semaphore &operator=(Semaphore &&) noexcept;

    void reset() noexcept;
    void down();
    void up();
    bool tryDown() noexcept;
    bool tryUp() noexcept;

private:
    typedef detail::SemaphoreWaiter Waiter;

    Scheduler *const scheduler_;
    List downWaiterList_;
    List upWaiterList_;
    const std::intmax_t initialValue_;
    const std::intmax_t minValue_;
    const std::intmax_t maxValue_;
    std::intmax_t value_;

    void initialize() noexcept;
    void move(Semaphore *) noexcept;
#ifdef SIREN_WITH_DEBUG
    bool isWaited() const noexcept;
#endif
    void downWaiterWakes() noexcept;
    void downWaiterSleeps() noexcept;
    void upWaiterWakes() noexcept;
    void upWaiterSleeps() noexcept;
};

} // namespace siren
