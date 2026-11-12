#include <cstddef>
#include <utility>
#include <limits>
#include <stdexcept>

struct p_t
{
  int x;
  int y;
};

class Ints
{
public:
  Ints();
  Ints(size_t size, int value = 0);
  Ints(const int * arr, size_t size);
  Ints(const Ints & rhs);
  Ints(Ints && rhs) noexcept;
  Ints & operator=(const Ints & rhs);
  Ints & operator=(Ints && rhs) noexcept;
  Ints operator+(const Ints & rhs) const;
  ~Ints();

  size_t getSize() const noexcept;
  void set(size_t id, int i) noexcept;
  int get(size_t id) const noexcept;
  void setSafe(size_t id, int i);
  int getSafe(size_t id) const;

  Ints append(int i) const;
  Ints append(const Ints & b) const;

  static Ints unitializatedInts(size_t size);

private:
  void resize();
  size_t capacity_;
  size_t size_;
  int * data_;
};

Ints::Ints():
  capacity_(0),
  size_(0),
  data_(nullptr)
{}

Ints::Ints(size_t size, int value):
  capacity_(size),
  size_(size),
  data_(size ? new int [size] : nullptr)
{
  for (size_t i = 0; i < size_; ++i)
  {
    data_[i] = value;
  }
}

Ints::Ints(const int * arr, size_t size):
  capacity_(size),
  size_(size),
  data_(size ? new int [size] : nullptr)
{
  if (size > 0 && arr == nullptr)
  {
    delete [] data_;
    throw std::invalid_argument("Null pointer passed");
  }

  for (size_t i = 0; i < size_; ++i)
  {
    data_[i] = arr[i];
  }
}

Ints Ints::unitializatedInts(size_t size)
{
  Ints temp;
  temp.size_ = 0;
  temp.capacity_ = size;
  temp.data_ = size ? new int [size] : nullptr;

  return temp;
}

Ints::Ints(const Ints & rhs):
  capacity_(rhs.capacity_),
  size_(rhs.size_),
  data_(capacity_ ? new int [capacity_] : nullptr)
{
  for (size_t i = 0; i < size_; ++i)
  {
    data_[i] = rhs.data_[i];
  }
}

Ints::Ints(Ints && rhs) noexcept:
  capacity_(rhs.capacity_),
  size_(rhs.size_),
  data_(rhs.data_)
{
  rhs.capacity_ = 0;
  rhs.size_ = 0;
  rhs.data_ = nullptr;
}

Ints & Ints::operator=(const Ints & rhs)
{
  if (this == &rhs)
  {
    return *this;
  }

  Ints temp(rhs);

  std::swap(capacity_, temp.capacity_);
  std::swap(size_, temp.size_);
  std::swap(data_, temp.data_);

  return *this;
}

Ints & Ints::operator=(Ints && rhs) noexcept
{
  if (this != &rhs)
  {
    delete [] data_;

    capacity_ = rhs.capacity_;
    size_ = rhs.size_;
    data_ = rhs.data_;

    rhs.data_ = nullptr;
    rhs.size_ = 0;
    rhs.capacity_ = 0;
  }

  return *this;
}

Ints Ints::operator+(const Ints & rhs) const
{
  if (getSize() != rhs.getSize())
  {
    throw std::invalid_argument("Arrays have different size");
  }

  Ints result(getSize(), 0);
  for (size_t i = 0; i < getSize(); ++i)
  {
    int a = get(i);
    int b = rhs.get(i);
    if ((b > 0 && a > std::numeric_limits< int >::max() - b)
        || (b < 0 && a < std::numeric_limits< int >::min() - b))
    {
      throw std::overflow_error("Overflow");
    }

    result.set(i, a + b);
  }

  return result;
}

Ints::~Ints()
{
  delete [] data_;
}

size_t Ints::getSize() const noexcept
{
  return size_;
}

void Ints::set(size_t id, int i) noexcept
{
  data_[id] = i;
}

int Ints::get(size_t id) const noexcept
{
  return data_[id];
}

void Ints::setSafe(size_t id, int i)
{
  if (id >= size_)
  {
    throw std::out_of_range("Invalid index");
  }

  set(id, i);
}

int Ints::getSafe(size_t id) const
{
  if (id >= size_)
  {
    throw std::out_of_range("Invalid index");
  }

  return get(id);
}

void Ints::resize()
{
  size_t newCapacity = 1;
  if (capacity_ > 0)
  {
    if (capacity_ > std::numeric_limits< size_t >::max() / 2)
    {
      throw std::overflow_error("Overflow");
    }

    newCapacity = capacity_ * 2;
  }

  int * newData = new int [newCapacity];
  for (size_t i = 0; i < size_; ++i)
  {
    newData[i] = data_[i];
  }

  delete [] data_;
  data_ = newData;
  capacity_ = newCapacity;
}

Ints Ints::append(int i) const
{
  Ints result(*this);

  if (result.capacity_ < result.size_ + 1)
  {
    result.resize();
  }

  result.data_[result.size_] = i;
  ++result.size_;

  return result;
}

Ints Ints::append(const Ints & b) const
{
  if (b.size_ > std::numeric_limits< size_t >::max() - size_)
  {
    throw std::overflow_error("Overflow");
  }

  Ints result = unitializatedInts(size_ + b.size_);
  for (size_t i = 0; i < size_; ++i)
  {
    result.data_[i] = data_[i];
  }

  for (size_t i = 0; i < b.size_; ++i)
  {
    result.data_[size_ + i] = b.data_[i];
  }

  result.size_ = size_ + b.size_;

  return result;
}

class Points
{
public:
  Points();
  Points(const Ints & data);
  Points(size_t size, p_t value);

  size_t getSize() const noexcept;
  p_t get(size_t id) const;
  void set(size_t id, p_t p);
  Points append(p_t p) const;

private:
  Ints data_points;
};

Points::Points():
  data_points()
{}

Points::Points(const Ints & data):
  data_points(data)
{
  if (data_points.getSize() % 2 != 0)
  {
    throw std::logic_error("Odd number of coordinates");
  }
}

Points::Points(size_t size, p_t value):
  data_points(size * 2, 0)
{
  for (size_t i = 0; i < size; ++i)
  {
    data_points.set(i * 2, value.x);
    data_points.set(i * 2 + 1, value.y);
  }
}

size_t Points::getSize() const noexcept
{
  return data_points.getSize() / 2;
}

p_t Points::get(size_t id) const
{
  if (id >= getSize())
  {
    throw std::out_of_range("Invalid index");
  }

  return p_t{ data_points.get(id * 2), data_points.get(id * 2 + 1) };
}

void Points::set(size_t id, p_t p)
{
  if (id >= getSize())
  {
    throw std::out_of_range("Invalid index");
  }

  data_points.set(id * 2, p.x);
  data_points.set(id * 2 + 1, p.y);
}

Points Points::append(p_t p) const
{
  size_t const size = data_points.getSize();
  if (size > std::numeric_limits< size_t >::max() - 2)
  {
    throw std::logic_error("Overflow");
  }

  Ints tmp(size + 2);
  for (size_t i = 0; i < size; ++i)
  {
    tmp.set(i, data_points.get(i));
  }

  tmp.set(size, p.x);
  tmp.set(size + 1, p.y);

  return tmp;
}

class Plot
{
public:
  void set(const int x, const int y);
  int calc(int x) const;
  int solve(Ints & buf, int y) const;
  Points append(p_t p) const;

private:
  Ints data_plot;
};

void Plot::set(const int x, const int y)
{
  for (size_t i = 0; i < data_plot.getSize(); i += 2)
  {
    if (data_plot.get(i) == x)
    {
      data_plot.set(i + 1, y);
      return;
    }
  }

  data_plot = data_plot.append(x);
  data_plot = data_plot.append(y);
}

int Plot::calc(const int x) const
{
  for (size_t i = 0; i < data_plot.getSize(); i += 2)
  {
    if (data_plot.get(i) == x)
    {
      return data_plot.get(i + 1);
    }
  }

  throw std::logic_error("Point not found");
}

int Plot::solve(Ints & buf, const int y) const
{
  int cnt = 0;
  for (size_t i = 1; i < data_plot.getSize(); i += 2)
  {
    if (y == data_plot.get(i))
    {
      buf = buf.append(data_plot.get(i - 1));
      ++cnt;
    }
  }

  return cnt;
}

Points Plot::append(const p_t p) const
{
  size_t const size = data_plot.getSize();
  if (size > std::numeric_limits< size_t >::max() - 2)
  {
    throw std::logic_error("Overflow");
  }

  Ints tmp(size + 2);
  for (size_t i = 0; i < size; ++i)
  {
    tmp.set(i, data_plot.get(i));
  }

  tmp.set(size, p.x);
  tmp.set(size + 1, p.y);

  return tmp;
}

class Mtx
{
public:
  Mtx();
  Mtx(size_t m, size_t n);
  Mtx(size_t m, size_t n, const Ints & a);
  ~Mtx() = default;

  void set(size_t i, size_t j, int value);
  int get(size_t i, size_t j) const;
  Mtx sum(const Mtx & oth) const;
  Mtx mul(const Mtx & oth) const;
  Ints getCol(size_t id) const;
  Ints getRow(size_t id) const;
  Mtx transpose() const;
  Mtx getMinor(size_t row, size_t col) const;
  int det() const;
  Mtx add_row(int init) const;

private:
  size_t m;
  size_t n;
  Ints data;
};

Mtx::Mtx():
  m(0),
  n(0),
  data()
{}

Mtx::Mtx(size_t m, size_t n):
  m(m),
  n(n),
  data()
{
  if (n != 0 && m > std::numeric_limits< size_t >::max() / n)
  {
    throw std::logic_error("Overflow");
  }

  data = Ints(m * n, 0);
}

Mtx::Mtx(size_t m, size_t n, const Ints & a):
  m(m),
  n(n),
  data(a)
{
  if (n != 0 && m > std::numeric_limits< size_t >::max() / n)
  {
    throw std::logic_error("Overflow");
  }

  if (m * n != a.getSize())
  {
    throw std::invalid_argument("Data size does not match matrix dimensions");
  }
}

void Mtx::set(size_t i, size_t j, int value)
{
  if (i >= m || j >= n)
  {
    throw std::out_of_range("Invalid index");
  }

  data.set(i * n + j, value);
}

int Mtx::get(size_t i, size_t j) const
{
  if (i >= m || j >= n)
  {
    throw std::out_of_range("Invalid index");
  }

  return data.get(i * n + j);
}

Mtx Mtx::sum(const Mtx & oth) const
{
  if (m != oth.m || n != oth.n)
  {
    throw std::invalid_argument("Matrices have different size");
  }

  Mtx res(m, n);
  res.data = data + oth.data;

  return res;
}

Ints Mtx::getCol(size_t id) const
{
  if (id >= n)
  {
    throw std::out_of_range("Invalid column index");
  }

  Ints res = Ints::unitializatedInts(m);
  for (size_t i = 0; i < m; ++i)
  {
    res = res.append(data.get(i * n + id));
  }

  return res;
}

Ints Mtx::getRow(size_t id) const
{
  if (id >= m)
  {
    throw std::out_of_range("Invalid row index");
  }

  Ints res = Ints::unitializatedInts(n);
  for (size_t i = 0; i < n; ++i)
  {
    res = res.append(data.get(id * n + i));
  }

  return res;
}

Mtx Mtx::mul(const Mtx & oth) const
{
  if (n != oth.m)
  {
    throw std::invalid_argument("Invalid matrices size");
  }

  Mtx res(m, oth.n);
  for (size_t i = 0; i < m; ++i)
  {
    Ints row = getRow(i);
    for (size_t j = 0; j < oth.n; ++j)
    {
      Ints col = oth.getCol(j);
      int sm = 0;
      for (size_t z = 0; z < n; ++z)
      {
        sm += row.get(z) * col.get(z);
      }

      res.data.set(i * oth.n + j, sm);
    }
  }

  return res;
}

Mtx Mtx::transpose() const
{
  Mtx res(n, m);
  for (size_t i = 0; i < m; ++i)
  {
    for (size_t j = 0; j < n; ++j)
    {
      res.data.set(j * m + i, data.get(i * n + j));
    }
  }

  return res;
}

Mtx Mtx::getMinor(size_t row, size_t col) const
{
  if (m == 0 || n == 0 || row >= m || col >= n)
  {
    throw std::out_of_range("Invalid index");
  }

  Mtx res(m - 1, n - 1);
  size_t id = 0;
  for (size_t i = 0; i < m; ++i)
  {
    if (i == row)
    {
      continue;
    }

    for (size_t j = 0; j < n; ++j)
    {
      if (j == col)
      {
        continue;
      }

      res.data.set(id, data.get(i * n + j));
      ++id;
    }
  }

  return res;
}

int Mtx::det() const
{
  if (m != n)
  {
    throw std::logic_error("Matrix is not square");
  }

  if (m == 0)
  {
    return 1;
  }

  if (m == 1)
  {
    return data.get(0);
  }

  int res = 0;
  int sign = 1;
  for (size_t j = 0; j < n; ++j)
  {
    res += sign * data.get(j) * getMinor(0, j).det();
    sign = -sign;
  }

  return res;
}

Mtx Mtx::add_row(int init) const
{
  if (m == std::numeric_limits< size_t >::max())
  {
    throw std::overflow_error("Overflow");
  }

  Mtx res;
  res.m = m + 1;
  res.n = n;
  res.data = data.append(Ints(n, init));

  return res;
}

int main()
{
  int arr[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  Ints a(arr, 9);
  Mtx mtx1(3, 3, a);
  (void)mtx1;

  return 0;
}
