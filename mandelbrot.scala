

object App {
  def main(args: Array[String]): Unit = {
    val range = List.range(0, 100)
    val coords = range zip range

    val fractal = coords.par.map( case(x,y) => {
      var zx = 0.0f
      var zy = 0.0f

      var zx_sqr = 0.0f
      var zy_sqr = 0.0f

      var iter = 0

      while (zx_sqr + zy_sqr < 4 && iter < 100) {
        val zxzy = zx + zy
        val zx0 = (zx_sqr) - (zy_sqr) + x

        zy = (zxzy) * (zxzy)-zx_sqr - zy_sqr + y
        zx = zx0

        zx_sqr = zx * zx
        zy_sqr = zy * zy

        iter ++;
      }

      iter
    })
  }
}
