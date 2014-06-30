assert 'UTF-8 detect' do
  det = UCharDet.new
  det.handle 'こんにちわ世界'
  det.end
  assert_equal 'UTF-8', det.result
end

assert 'result before end' do
  det = UCharDet.new
  assert_raise(RuntimeError) { det.result }
end
